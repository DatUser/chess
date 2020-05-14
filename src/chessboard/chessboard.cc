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


    std::vector<Move> add_in_vector(std::vector<Move> v1, std::vector<Move> v2)
    {
        for (Move mv : v2)
        {
            v1.push_back(mv);
        }

        return v1;
    }


    std::vector<Move> Chessboard::generate_legal_moves()
    {
        Rule rules = Rule();
        std::vector<Move> all_moves;

        std::vector<Move> pawns = rules.generate_pawn_moves(*this);
        all_moves = add_in_vector(all_moves, pawns);

        std::vector<Move> king = rules.generate_king_moves(*this);
        all_moves = add_in_vector(all_moves, king);

        std::vector<Move> bishops = rules.generate_bishop_moves(*this);
        all_moves = add_in_vector(all_moves, bishops);

        std::vector<Move> rooks = rules.generate_rook_moves(*this);
        all_moves = add_in_vector(all_moves, rooks);

        std::vector<Move> queen = rules.generate_queen_moves(*this);
        all_moves = add_in_vector(all_moves, queen);

        std::vector<Move> knights = rules.generate_knight_moves(*this);
        all_moves = add_in_vector(all_moves, knights);

        return all_moves;
    }
}
