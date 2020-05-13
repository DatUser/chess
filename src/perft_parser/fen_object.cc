#include "fen_object.hh"

namespace perft_parser {

    FenObject::operator[](Position position)
    {
        int rank = utype(position.rank_get());
        int file = utype(position.file_get());

        return ranks[rank][file];
    }
}
