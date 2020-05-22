#include "perft_parser.hh"
#include <listener/listener-manager.hh>
#include "listener.hh"

namespace perft_parser {

    using ListenerManager = listener::ListenerManager;

    PerftObject parse_perft(string input)
    {
        stringstream ss(input);
        string item;
        vector<string> items;
        while (getline(ss, item, ' ')) {
            items.push_back(item);
        }
        if (items[0].compare("position") == 0) {
            items.erase(items.begin());
        }
        if (items[0].compare("fen") == 0) {
            items.erase(items.begin());
        }
        int depth = stoi(items[items.size() - 1]);
        items.pop_back();

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
                black_danger,
                items[0]);

        auto c = Chessboard(board, items);
        ListenerManager::instance().chessboard_set(&c);
        auto listeners = ListenerManager::instance().listeners_get();
        for (auto listener : listeners)
            listener->register_board(c);
        return PerftObject(c, depth);
    }

    // ASK YAKA ABOUT CHESSBOARD CONSTRUCTOR
    Chessboard parse_fen(vector<string> splited_input)
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
                black_danger,
                splited_input[0]);
        auto c = Chessboard(board,splited_input);
        ListenerManager::instance().chessboard_set(&c);
        auto listeners = ListenerManager::instance().listeners_get();
        for (auto listener : listeners)
            listener->register_board(c);

        return c;
    }
}
