#pragma once

#include <memory>

#include "../bitboard/all.hh"
#include "../all.hh"
#include "../utils/all.hh"

using namespace std;

namespace board {
    using bit = Bitboard&;

    class Board {

    public:
        Board(
            bit king_wb,
            bit queen_wb,
            bit knight_wb,
            bit bishop_wb,
            bit rook_wb,
            bit pawn_wb,
            bit king_bb,
            bit queen_bb,
            bit knight_bb,
            bit bishop_bb,
            bit rook_bb,
            bit pawn_bb,
            bit white_occupied,
            bit black_occupied,
            bit occupied_board,
            bit white_danger,
            bit black_danger
        );

        Board(
            bit king_wb,
            bit queen_wb,
            bit knight_wb,
            bit bishop_wb,
            bit rook_wb,
            bit pawn_wb,
            bit king_bb,
            bit queen_bb,
            bit knight_bb,
            bit bishop_bb,
            bit rook_bb,
            bit pawn_bb,
            bit white_occupied_board,
            bit black_occupied_board,
            bit occupied_board,
            bit white_danger,
            bit black_danger,
            string ranks
        );

        Board(Board& board);
        Board& operator=(Board board);

        /* Refreshes the occupied_board */
        void refresh_occupied();

        /* Compute white danger */
        void compute_danger();
        void compute_white_danger();
        void compute_black_danger();

        /* Get the state of a case at coordinate given in input */
        opt_piecetype_t is_occupied(const Position& position, Color color);
        bool is_occupied(bit board, const Position& position) const;
        //bool is_occupied(Position position);

        /* Fait le move fait le move */
        void do_move(Move move, Color color);
        bool is_check(bool white_turn_);

        bit& white_occupied_board_get();
        bit& black_occupied_board_get();
        bit& occupied_board_get();
        // DEBUG TIER
        void print();

        vector<Position> get_white_king();
        vector<Position> get_white_queen();
        vector<Position> get_white_knight();
        vector<Position> get_white_bishop();
        vector<Position> get_white_rook();
        vector<Position> get_white_pawn();

        vector<Position> get_black_king();
        vector<Position> get_black_queen();
        vector<Position> get_black_knight();
        vector<Position> get_black_bishop();
        vector<Position> get_black_rook();
        vector<Position> get_black_pawn();

        // Bitboards needed for the white side
        bit king_wb;
        bit queen_wb;
        bit knight_wb;
        bit bishop_wb;
        bit rook_wb;
        bit pawn_wb;

        // Bitboards needed for the black side
        bit king_bb;
        bit queen_bb;
        bit knight_bb;
        bit bishop_bb;
        bit rook_bb;
        bit pawn_bb;

        // Occupied Bitboard
        bit white_occupied_board;
        bit black_occupied_board;
        bit occupied_board;

        // Danger cases Bitboard
        bit white_danger;
        bit black_danger;

    private:
        // Methods needed to compute the danger grids
        void compute_king_danger(bit board, int power);
        void compute_queen_danger(bit board, bit ally,
                                  bit enemy, bit queens);
        void compute_bishop_danger(bit board, bit ally,
                                   bit enemy, bit bishops);
        void compute_rook_danger(bit board, bit ally,
                                 bit enemy, bit rooks);
        void compute_knight_danger(bit board, bit ally,
                                   bit knights);
        void compute_pawn_danger(bit board, bit ally,
                                 bit pawns, bool white);

        void compute_lines(bit board, bit ally,
                                  bit enemy, int power);
        void compute_diagonals(bit board, bit ally,
                                  bit enemy, int power);

        void put_piece(Color color, PieceType piece, Position pos);
    };
    #include "board.hxx"
}
