#pragma once

#include "fen_rank.hh"

#include "../color.hh"
#include "../position.hh"

using namespace board;
using namespace std;

namespace perft_parser {

    class FenObject {
        pair<PieceType, Color> operator[](Position position);
        Color side_to_move_get();
        vector<char> castling_get();
        Position en_passant_target_get();

    private:
        vector<FenRank> ranks_;
        Color side_to_move_;
        vector<char> castling_;
        Position en_passant_target_;
    };
}
