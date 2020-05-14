#pragma once

namespace perft_parser {

    inline Chessboard& PerftObject::chessboard_get() { return chessboard_; }
    inline int PerftObject::depth_get() { return depth_; }
}
