#include "all.hh"
#include "pgn-parser.hh"
#include "chessboard/chessboard.hh"
#include "move.hh"

namespace board
{
    std::vector<Move> get_moves_from_pgn(std::string& file)
    {
        std::vector<PgnMove> pgn_moves = pgn_parser::parse_pgn(file);

        std::vector<Move> result;

        for (PgnMove pm : pgn_moves)
        {
            Move mv = Move(pm.get_start(), pm.get_end());
            mv.piece_set(pm.get_piece());
            if (pm.get_promotion() != std::nullopt)
            {
                mv.promotion_set(pm.get_promotion().value());
            }
            if (pm.get_capture())
            {
                mv.capture_set(pm.get_piece());
            }

            result.push_back(mv);
        }

        return result;
    }

    Chessboard generate_chessboard(std::vector<Move> moves)
    {
        Chessboard chessboard = Chessboard();
        for (Move m : moves)
        {
            m.piece_set(PieceType::ROOK);
        }

        return chessboard;
    }
}
