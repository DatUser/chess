#include "fen_object.hh"

namespace perft_parser {

    pair<PieceType, Color> FenObject::operator[](Position position)
    {
        int rank = utils::utype(position.rank_get());
        int file = utils::utype(position.file_get());

        return ranks_[rank][file];
    }
}
