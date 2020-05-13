#include "chessboard.hh"
#include "rule.hh"

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


    std::vector<Move> Chessboard::generate_legal_moves()
    {
        Rule rules = Rule();

        std::vector<Move> result;
        result = rules.generate_pawn_moves(*this);

        return result;
    }
}
