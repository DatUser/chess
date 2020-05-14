#pragma once

#include <vector>

#include "../color.hh"
#include "../piece-type.hh"

using namespace std;
using namespace board;

namespace perft_parser {

    class FenRank {
    public:
        pair<PieceType, Color> operator[](size_t index);

    private:
        vector<pair<PieceType, Color>> pieces_;
    };
}
