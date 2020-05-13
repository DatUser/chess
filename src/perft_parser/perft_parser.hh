#pragma once

#include "perft_object.hh"
#include "fen_object.hh"

using namespace board;
using namespace std;

namespace perft_parser {

    PerftObject parse_perft(string input);
    FenObject parse_fen(vector<string> splited_input);
}
