#pragma once
#include "pgn-move.hh"

    inline const Position PgnMove::get_start()
    {
        return start_;
    }

    inline const Position PgnMove::get_end()
    {
        return end_;
    }

    inline PieceType PgnMove::get_piece()
    {
        return piece_;
    }

    inline PgnMove::opt_piece_t PgnMove::get_promotion()
    {
        return promotion_;
    }

    inline bool PgnMove::get_capture()
    {
        return capture_;
    }

    inline ReportType PgnMove::get_report()
    {
        return report_;
    }
