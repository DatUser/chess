#include "all.hh"
#include "pgn-parser.hh"
#include "chessboard/chessboard.hh"
#include "move.hh"

namespace board
{
    std::vector<Move> get_moves_from_pgn(const std::string& file);
    Chessboard generate_chessboard(std::vector<Move> moves);
}
