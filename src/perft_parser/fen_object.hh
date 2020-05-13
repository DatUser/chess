#pragma once

#include "fen_rank.hh"

using namespace board;
using namespace std;

namespace perft_parser {

    class FenObject {
        operator[](Position position);
        Color side_to_move_get();
        vector<char> castling_get();
        Position en_passant_target_get();

    private:
        vector<FenRank> ranks_;
        Color side_to_move_;
        vector<char> castling_;
        Position en_passant_target_;
    }
}
