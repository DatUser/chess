#pragma once

#include "../all.hh"
#include "../utils/all.hh"
#include "../board/all.hh"
#include "../bitboard/all.hh"

namespace board {
    class Chessboard {
    public:
        Chessboard();

    private:
        Board board_;
    };
}
