#pragma once

#include "../chessboard/chessboard.hh"
#include "../board/board.hh"
#include "../move.hh"

using namespace std;
using namespace board;

namespace chess_engine {

    Move search(Chessboard& board, int depth);
    int evaluate(Chessboard& board);
}
