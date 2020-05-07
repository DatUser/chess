#pragma once

#include <iostream>
#include <optional>
#include "../all.hh"
#include "../utils/all.hh"

namespace board {

    class Bitboard {
    public:
        Bitboard(PieceType type, Color color);
        Bitboard(unsigned long long int board);
        Bitboard(Bitboard& other);
        Bitboard();

        void print();

        bool move(Position start_pos, Position end_pos);

        void case_set(Position position);

        Bitboard operator|=(Bitboard other);
        Bitboard operator=(Bitboard& other);

        // Inlined methods
        unsigned long long int board_get();
        void board_set(Bitboard other);
        void board_set(unsigned long long int other);

    private:
        unsigned long long int board_;
        const std::optional<PieceType> type_;
    };
}
#include "bitboard.hxx"
