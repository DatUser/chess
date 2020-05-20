#pragma once

#include <stdlib.h>

#include "all.hh"

namespace board
{
    class Move {
        public:
            Move(Position start, Position end);
            ~Move() = default;

            std::pair<Position, Position> move_get() const;

            void piece_set(PieceType piece);

            PieceType piece_get();

            void promotion_set(PieceType promotion);

            PieceType promotion_get();

            void capture_set(PieceType capture);

            PieceType capture_get();

            void double_pawn_push_set(bool double_pawn_push);

            bool double_pawn_push_get();

            void king_castling_set(bool king_castling);

            bool king_castling_get();

            void queen_castling_set(bool queen_castling);

            bool queen_castling_get();

            void en_passant_set(bool en_passant);

            bool en_passant_get();

            bool operator==(Move& move);

        private:
            const std::pair<Position, Position> move_;
            PieceType piece_ = PieceType::NONE;
            PieceType promotion_ = PieceType::NONE;
            PieceType capture_ = PieceType::NONE;
            bool double_pawn_push_ = false;
            bool king_castling_ = false;
            bool queen_castling_ = false;
            bool en_passant_ = false;
    };
}

#include "move.hxx"
