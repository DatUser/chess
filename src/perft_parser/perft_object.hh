#pragma once

#include "fen_object.hh"

#include "../chessboard/chessboard.hh"

namespace perft_parser {

    class PerftObject {
    public:
        PerftObject(Chessboard chessboard, int depth);

        Chessboard& chessboard_get();
        int depth_get();

    private:
        Chessboard chessboard_;
        int depth_;
    };
}

#include "perft_object.hxx"
