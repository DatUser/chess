#include <iostream>
#include <fstream>

#include "parser/option-parser.hh"
#include "perft_parser/perft_parser.hh"
#include "AI/ai.hh"
#include "uci.hh"

#include "generate-chessboard.hh"
#include <parser/option-parser.hh>
//#define _GNU_SOURCE
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
        chess_engine::search(chessboard, 4);
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
        chess_engine::search(pobject.chessboard_get(), 4);
        auto bestmove = chess_engine::search(pobject.chessboard_get(), 2);
        /*for (long unsigned int i = 0; i < moves.size(); i++) {
            auto move = moves[i];
            auto brank = utils::utype(move.move_get().first.rank_get()) + 1;
            char bfile = utils::utype(move.move_get().first.file_get()) + 'a';
            auto erank = utils::utype(move.move_get().second.rank_get()) + 1;
            char efile = utils::utype(move.move_get().second.file_get()) + 'a';
            std::cout << bfile <<  brank << " " << efile << erank << "\n";
        }*/
        /*for (int i = 0; i < 120; i++) {
            pobject.chessboard_get().print();
            std::cout << chess_engine::evaluate(pobject.chessboard_get()) << "\n";
            auto move = chess_engine::search(pobject.chessboard_get(), 2);
            auto brank = utils::utype(move.move_get().first.rank_get()) + 1;
            char bfile = utils::utype(move.move_get().first.file_get()) + 'a';
            auto erank = utils::utype(move.move_get().second.rank_get()) + 1;
            char efile = utils::utype(move.move_get().second.file_get()) + 'a';
            std::cout << bfile <<  brank << " " << efile << erank << "\n";
    } else {
        Chessboard chessboard;
        chess_engine::search(chessboard, 4);
            std::cout << bfile <<  brank << " " << efile << erank << "\n";
            pobject.chessboard_get().do_move(move);
            pobject.chessboard_get().setWhiteTurn(!pobject.chessboard_get().isWhiteTurn());
            std::cout << "i: " << i << "\n";
        }*/
    }
    else
    {
        ai::init("EscanorEngine");
        std::string line;
        ofstream file("moves");
        if (not file.is_open()) {
            cerr << "cannot open file" << endl;
            exit(1);
        }
        while ((line = ai::get_board()).compare(""))
        {
            file << line << "\n";
            file.flush();
            stringstream ss(line);
            string item;
            string move;
            vector<string> items;
            while (getline(ss, item, ' ')) {
                items.push_back(item);
            }
            move = items[items.size() - 1];
            auto to_play = utils::to_move(move);

            if (items[0] == "position")
                items.erase(items.begin());
            if (items[0] == "fen")
                items.erase(items.begin());

            auto board = board::Chessboard(items);
            board.do_move(to_play);
            auto bestmove = chess_engine::search(board, 2);

            auto best_str = pos_to_string(bestmove.move_get().first)
                                + pos_to_string(bestmove.move_get().second);

            file << best_str << "\n";
            file.flush();
            ai::play_move(best_str);
        }

        file.close();
    }

    close_listeners();

    return 0;
}
