#pragma once

#include "../bitboard/all.hh"
#include "../all.hh"
#include "../utils/all.hh"

namespace board {

    class Board {
        Board();

        /* Refreshes the occupied_board */
        void refresh_occupied();

        /* Compute white danger */
        void compute_white_danger();
        void compute_black_danger();

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
        Bitboard occupied_board;

        // Danger cases Bitboard
        Bitboard white_danger;
        Bitboard black_danger;
    }
}
