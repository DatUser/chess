#include "all.hh"
#include "move.hh"

namespace board
{
    Move::Move(Position start, Position end)
    : move_(std::pair<Position, Position>(start, end))
    {}

    bool Move::operator==(Move& move)
    {
        std::pair<Position, Position> pos1 = move_get();
        std::pair<Position, Position> pos2 = move.move_get();
        if (pos1.first == pos2.first and pos1.second == pos2.second and
            piece_get() == move.piece_get() and
            promotion_get() == move.promotion_get() and
            capture_get() == move.capture_get() and
            double_pawn_push_get() == move.double_pawn_push_get() and
            king_castling_get() == move.king_castling_get() and
            queen_castling_get() == move.queen_castling_get() and
            en_passant_get() == move.en_passant_get())
        {
            return true;
        }

        return false;
    }

    Move Move::operator=(Move& move)
    {
        if (move == *this)
        {
            return move;
        }

        std::pair<Position, Position> pos = move.move_get();
        Move new_move = Move(pos.first, pos.second);
        new_move.piece_set(move.piece_get());
        new_move.promotion_set(move.promotion_get());
        new_move.capture_set(move.capture_get());
        new_move.double_pawn_push_set(move.double_pawn_push_get());
        new_move.king_castling_set(move.king_castling_get());
        new_move.queen_castling_set(move.queen_castling_get());
        new_move.en_passant_set(move.en_passant_get());
        return new_move;
    }
}
