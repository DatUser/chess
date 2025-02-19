#pragma once

#include "all.hh"
#include "move.hh"
#include "chessboard/chessboard.hh"
#include "board/board.hh"

#include <vector>

namespace board
{
    class Rule
    {
    public:
        Rule()
        {}

        ~Rule() = default;

        void generate_pawn_moves(Chessboard& chessboard,
                                    std::vector<Move>& moves);
        void generate_king_moves(Chessboard& chessboard,
                                    std::vector<Move>& moves);
        void generate_bishop_moves(Chessboard& chessboard,
                                    std::vector<Move>& moves);
        void generate_rook_moves(Chessboard& chessboard,
                                    std::vector<Move>& moves);
        void generate_queen_moves(Chessboard& chessboard,
                                    std::vector<Move>& moves);
        void generate_knight_moves(Chessboard& chessboard,
                                    std::vector<Move>& moves);
    };
}
