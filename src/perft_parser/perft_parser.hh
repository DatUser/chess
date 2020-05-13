#pragma once

#include <vector>

#include "../piece-type.hh"

using namespace board;
using namespace std;

namespace perft_parser {

    class FenRank {
    public:
        pair<PieceType, Color> operator[](size_t index);

    private:
        vector<pair<PieceType, Color>> pieces_;
    }
}
