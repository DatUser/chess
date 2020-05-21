#include <iostream>
#include <fstream>

#include "parser/option-parser.hh"
#include "perft_parser/perft_parser.hh"
#include "AI/ai.hh"
#include "uci.hh"

#include "generate-chessboard.hh"
#include <parser/option-parser.hh>
//#define _GNU_SOURCE
#include <algorithm>
#include <dlfcn.h>
#include <listener.hh>
#include <listener/listener-manager.hh>


using namespace std;

//CHECK RETURN VALUES

void open_listeners(OptionParser& parser) {

    auto paths = parser.listeners_get();
    std::vector<listener::Listener*> listeners;


    for (auto path : paths) {
        void* out = dlopen(path.c_str(), RTLD_LAZY);

        if (not out) {
            std::string err = dlerror();
            std::cout << err << std::endl;
        } else {
            const char* constr_list = "listener_create";
            void* symbol = dlsym(out, constr_list);
            listener::Listener* listener =
                        reinterpret_cast<listener::Listener*(*)()>(symbol)();
            if (not listener) {
                std::string err = dlerror();
                std::cout << err << std::endl;
            } else {
                listeners.push_back(listener);
            }
        }
    }

    listener::ListenerManager::instance().listeners_set(listeners);
}

void close_listeners() {
    auto listens = listener::ListenerManager::instance().listeners_get();
    for (auto listen : listens)
        dlclose(listen);
}

int main(int argc, char** argv) {

    OptionParser parser(argc, argv);
    open_listeners(parser);

    if (parser.help_get()) {
        std::cout << "-h [ --help ]             show usage"
            << std::endl
            << "--pgn                     arg path to the PGN game file"
            << std::endl
            << "-l [ --listeners ] args   list of paths to listener plugins"
            << std::endl
            << "--perft arg               path to a perft file"
            << std::endl;
    }
    else if (parser.pgn_get() != "")
    {
        std::vector<board::Move> moves =
                    board::get_moves_from_pgn(parser.pgn_get());
        board::Chessboard chessboard = board::generate_chessboard(moves);
        chess_engine::search(chessboard, 2);
    }
    else if (parser.perft_get().compare(""))
    {
        ifstream file;
        file.open(parser.perft_get());
        if (!file.is_open())
            return 1;
        string line;
        getline(file, line);
        auto pobject = perft_parser::parse_perft(line);
        auto moves = pobject.chessboard_get().generate_legal_moves();
        std::cout << moves.size() << std::endl;
    }
    else
    {
        ai::init("EscanorEngine");
        ofstream file("moves");
        std::string line;
        while ((line = ai::get_board()).compare(""))
        {
            file << line << std::endl;
            file.flush();
            stringstream ss(line);
            string item;
            string move;
            vector<string> items;
            while (getline(ss, item, ' ')) {
                items.push_back(item);
            }

            if (items[0] == "position")
                items.erase(items.begin());
            if (items[0] == "fen")
                items.erase(items.begin());

            Chessboard board;
            if (items[0] == "startpos")
            {
                auto board = Chessboard();
                string temp = "moves";
                auto begin_moves = find(items.begin(), items.end(), temp);
                do {
                    begin_moves++;
                    if (begin_moves->size() == 0 or ((*begin_moves)[0] < 'a'
                                and (*begin_moves)[0] > 'f'))
                        break;
                    move = *begin_moves;
                    auto to_play = board.to_move(move);
                    board.do_move(to_play);
                } while (begin_moves != items.end());
                auto bestmove = chess_engine::search(board, 2);

                auto best_str = pos_to_string(bestmove.move_get().first)
                    + pos_to_string(bestmove.move_get().second);
                ai::play_move(best_str);
            }
            else
            {
                auto board = Chessboard(items);
                string temp = "moves";
                auto begin_moves = find(items.begin(), items.end(), temp);
                do {
                    begin_moves++;
                    if (begin_moves->size() == 0 or ((*begin_moves)[0] < 'a'
                                and (*begin_moves)[0] > 'f'))
                        break;
                    move = *begin_moves;
                    auto to_play = board.to_move(move);
                    board.do_move(to_play);
                } while (begin_moves != items.end());
                auto bestmove = chess_engine::search(board, 2);

                auto best_str = pos_to_string(bestmove.move_get().first)
                    + pos_to_string(bestmove.move_get().second);
                ai::play_move(best_str);
            }
        }
    }

    close_listeners();

    return 0;
}
