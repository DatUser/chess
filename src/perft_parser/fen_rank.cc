c
#include "fen_rank.hh"

namespace perft_parser {

    pair<PieceType, Color> FenRank::operator[](size_t index)
    {
        return pieces_[index];
    }
}
