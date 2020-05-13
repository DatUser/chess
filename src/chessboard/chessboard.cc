#include "chessboard.hh"

namespace board {
    Chessboard::Chessboard() {
        board_ = Board();
    }

    void Chessboard::do_move(Move move)
    {
        if (white_turn_)
            board_.do_move(move, Color::WHITE);
        else
            board_.do_move(move, Color::BLACK);
    }

    bool is_check()
    {
        board_.is_check(white_turn_);
    }
}
