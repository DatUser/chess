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
        for (auto move : moves)
        {
            char fileb = (int) move.move_get().first.file_get() + 'a';
            char rankb = (int) move.move_get().first.rank_get() + '1';
            char filee = (int) move.move_get().second.file_get() + 'a';
            char ranke = (int) move.move_get().second.rank_get() + '1';
            std::cout << fileb << rankb << " " << filee << ranke << "\n";
        }
        std::cout << moves.size() << std::endl;
    }
    else
    {
        ai::init("EscanorEngine");
        std::string line;
        while ((line = ai::get_board()).compare(""))
        {
            ofstream file("moves");
            file << line << "\n";
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

            if (items[0] == "startpos")
            {
                auto king_wb = Bitboard(PieceType::KING, Color::WHITE);
                auto queen_wb = Bitboard(PieceType::QUEEN, Color::WHITE);
                auto knight_wb = Bitboard(PieceType::KNIGHT, Color::WHITE);
                auto bishop_wb = Bitboard(PieceType::BISHOP, Color::WHITE);
                auto rook_wb = Bitboard(PieceType::ROOK, Color::WHITE);
                auto pawn_wb = Bitboard(PieceType::PAWN, Color::WHITE);

                auto king_bb = Bitboard(PieceType::KING, Color::BLACK);
                auto queen_bb = Bitboard(PieceType::QUEEN, Color::BLACK);
                auto knight_bb = Bitboard(PieceType::KNIGHT, Color::BLACK);
                auto bishop_bb = Bitboard(PieceType::BISHOP, Color::BLACK);
                auto rook_bb = Bitboard(PieceType::ROOK, Color::BLACK);
                auto pawn_bb = Bitboard(PieceType::PAWN, Color::BLACK);

                auto occupied_board = Bitboard();

                auto white_occupied_board = Bitboard();
                auto black_occupied_board = Bitboard();

                auto white_danger = Bitboard();
                auto black_danger = Bitboard();

                auto board = Board(
                        king_wb,
                        queen_wb,
                        knight_wb,
                        bishop_wb,
                        rook_wb,
                        pawn_wb,
                        king_bb,
                        queen_bb,
                        knight_bb,
                        bishop_bb,
                        rook_bb,
                        pawn_bb,
                        white_occupied_board,
                        black_occupied_board,
                        occupied_board,
                        white_danger,
                        black_danger
                        );
                auto chessboard = Chessboard(board);
                string temp = "moves";
                auto begin_moves = find(items.begin(), items.end(), temp);
                do {
                    begin_moves++;
                    if (begin_moves->size() == 0 or ((*begin_moves)[0] < 'a'
                                and (*begin_moves)[0] > 'f'))
                        break;
                    move = *begin_moves;
                    auto to_play = chessboard.to_move(move);
                    chessboard.do_move(to_play);
                } while (begin_moves != items.end());
                auto bestmove = chess_engine::search(chessboard, 4);

                auto best_str = pos_to_string(bestmove.move_get().first)
                    + pos_to_string(bestmove.move_get().second);
                ai::play_move(best_str);
            }
            else
            {
                auto king_wb = Bitboard();
                auto queen_wb = Bitboard();
                auto knight_wb = Bitboard();
                auto bishop_wb = Bitboard();
                auto rook_wb = Bitboard();
                auto pawn_wb = Bitboard();

                auto king_bb = Bitboard();
                auto queen_bb = Bitboard();
                auto knight_bb = Bitboard();
                auto bishop_bb = Bitboard();
                auto rook_bb = Bitboard();
                auto pawn_bb = Bitboard();

                auto occupied_board = Bitboard();

                auto white_occupied_board = Bitboard();
                auto black_occupied_board = Bitboard();

                auto white_danger = Bitboard();
                auto black_danger = Bitboard();
                
                auto board = Board(
                        king_wb,
                        queen_wb,
                        knight_wb,
                        bishop_wb,
                        rook_wb,
                        pawn_wb,
                        king_bb,
                        queen_bb,
                        knight_bb,
                        bishop_bb,
                        rook_bb,
                        pawn_bb,
                        white_occupied_board,
                        black_occupied_board,
                        occupied_board,
                        white_danger,
                        black_danger,items[0]);
                auto chessboard = Chessboard(board, items);
                string temp = "moves";
                auto begin_moves = find(items.begin(), items.end(), temp);
                do {
                    begin_moves++;
                    if (begin_moves->size() == 0 or ((*begin_moves)[0] < 'a'
                                and (*begin_moves)[0] > 'f'))
                        break;
                    move = *begin_moves;
                    auto to_play = chessboard.to_move(move);
                    chessboard.do_move(to_play);
                } while (begin_moves != items.end());
                auto bestmove = chess_engine::search(chessboard, 4);

                auto best_str = pos_to_string(bestmove.move_get().first)
                    + pos_to_string(bestmove.move_get().second);
                ai::play_move(best_str);
            }
        }
    }

    close_listeners();

    return 0;
}
