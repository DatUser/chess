#pragma once

#include "fen_object.hh"

namespace perft_parser {

    class PerftObject {
    public:
        FenObject fen_get();
        int depth_get();

    private:
        FenObject fen_;
        int depth_;
    };
}
