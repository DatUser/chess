#pragma once

#include "../bitboard/all.hh"
#include "../all.hh"
#include "../utils/all.hh"

namespace board {

    class Board {

    public:
        Board();

        /* Refreshes the occupied_board */
        void refresh_occupied();

        /* Compute white danger */
        void compute_danger();
        void compute_white_danger();
        void compute_black_danger();

        /* Get the state of a case at coordinate given in input */
        bool is_occupied(Bitboard board, Position position);
        bool is_occupied(Position position);

    private:
        // Bitboards needed for the white side
        Bitboard king_wb;
        Bitboard queen_wb;
        Bitboard knight_wb;
        Bitboard bishop_wb;
        Bitboard rook_wb;
        Bitboard pawn_wb;

        // Bitboards needed for the black side
        Bitboard king_bb;
        Bitboard queen_bb;
        Bitboard knight_bb;
        Bitboard bishop_bb;
        Bitboard rook_bb;
        Bitboard pawn_bb;

        // Occupied Bitboard
        Bitboard white_occupied_board;
        Bitboard black_occupied_board;
        Bitboard occupied_board;

        // Danger cases Bitboard
        Bitboard white_danger;
        Bitboard black_danger;

        // Methods needed to compute the danger grids
        void compute_king_danger(Bitboard* board, Bitboard occ, int power);
        void compute_queen_danger(Bitboard* board, Bitboard ally,
                                  Bitboard enemy, int power);

        void compute_lines(Bitboard* board, Bitboard ally,
                                  Bitboard enemy, int power);
        void compute_diagonals(Bitboard* board, Bitboard ally,
                                  Bitboard enemy, int power);
    };
}
