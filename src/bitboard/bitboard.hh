#pragma once

#include <iostream>
#include <optional>
#include "../all.hh"

namespace board {

    class Bitboard {
    public:
        Bitboard(PieceType type, Color color);
        Bitboard();

        void print();

        bool move(Position start_pos, Position end_pos);

        long long int board_get();

    private:
        long long int board_;
        const std::optional<PieceType> type_;
    };
}
