#pragma once

#include <vector>

#include "../piece-type.hh"

namespace parsing {

    class FenRank {
    public:
        pair<PieceType, Color> operator[](size_t index);

    private:
        vector<pair<PieceType, Color>> pieces_;
    }
}
