#pragma once
#include "board.hh"

inline bit& Board::white_occupied_board_get() {
    return white_occupied_board;
}

inline bit& Board::black_occupied_board_get() {
    return black_occupied_board;
}

inline bit& Board::occupied_board_get() {
    return occupied_board;
}
