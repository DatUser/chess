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

        return result;
    }
}
