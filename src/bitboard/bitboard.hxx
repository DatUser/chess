#pragma once

namespace board {
    inline unsigned long long int Bitboard::board_get() { return board_; }

    inline void Bitboard::board_set(Bitboard other) { board_ = other.board_get(); }
    inline void Bitboard::board_set(unsigned long long int other) { board_ = other; }
}
