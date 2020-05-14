#pragma once
#include "board.hh"

inline shared_bit Board::white_occupied_board_get() {
    return white_occupied_board;
}

inline shared_bit Board::black_occupied_board_get() {
    return black_occupied_board;
}

inline shared_bit Board::occupied_board_get() {
    return occupied_board;
}
