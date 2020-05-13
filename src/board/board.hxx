#pragma once
#include "board.hh"

inline Bitboard& Board::white_occupied_board_get() {
    return *white_occupied_board;
}

inline Bitboard& Board::black_occupied_board_get() {
    return *black_occupied_board;
}

inline Bitboard* Board::occupied_board_get() {
    return occupied_board;
}
