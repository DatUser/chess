#include "all.hh"

class Move {
    public:
        Move(board::Position start, board::Position end);

        ~Move() = default;

    private:
        const std::pair<board::Position, board::Position> move_;
        board::PieceType piece_;
        board::PieceType promotion_;
        board::PieceType capture_;
        bool double_pawn_push_;
        bool king_castling_;
        bool queen_castling_;
        bool en_passant_;
};
