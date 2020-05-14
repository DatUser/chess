#pragma once

namespace perft_parser {

    inline Chessboard& PerftObject::chessboard_get() { return chessboard_; }
    inline int depth_get() { return depth_; }
}
