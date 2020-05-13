#include "chessboard.hh"

namespace board {
    Chessboard::Chessboard() { board_ = Board(); }

    void do_move(Move move)
    {
        if (white_turn_)
            board.do_move(move, Color::WHITE);
        else
            board.do_move(move, Color::BLACK);
    }
}
