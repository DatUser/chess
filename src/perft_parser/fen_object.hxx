#pragma once

namespace perft_parser {

    inline Color FenObject::side_to_move_get()
    {
        return side_to_move_;
    }

    inline vector<char> FenObject::castling_get()
    {
        return castling_;
    }

    inline Position FenObject::en_passant_target_get()
    {
        return en_passant_target_;
    }
}
