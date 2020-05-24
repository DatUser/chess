#pragma once

#include <array>
#include <optional>

namespace board
{
    /* PieceType is an enum representing every possible
     * piece type present on the board. The char associated
     * with each value is the ascii char representing the
     * piece on the board */
    enum class PieceType
    {
        QUEEN,
        ROOK,
        BISHOP,
        KNIGHT,
        PAWN,
        KING, // Last position allows better optimization (order-move)
        NONE
    };

    using opt_piecetype_t = std::optional<PieceType>;

    /* Can be useful to iterate over PieceTypes */
    constexpr std::array<PieceType, 6> piecetype_array{
        PieceType::QUEEN,  PieceType::ROOK, PieceType::BISHOP,
        PieceType::KNIGHT, PieceType::PAWN, PieceType::KING};

    inline PieceType char_to_piece(const char c)
    {
        switch (c)
        {
        case 'P':
        case 'p':
            return PieceType::PAWN;
        case 'N':
        case 'n':
            return PieceType::KNIGHT;
        case 'B':
        case 'b':
            return PieceType::BISHOP;
        case 'R':
        case 'r':
            return PieceType::ROOK;
        case 'Q':
        case 'q':
            return PieceType::QUEEN;
        case 'K':
        case 'k':
            return PieceType::KING;
        default:
            throw std::invalid_argument("Unknown piece");
        }
    }

    inline int value_piece(PieceType piece) {
        switch (piece)
                {
                    case PieceType::KING:
                        return 99999;
                    case PieceType::QUEEN:
                        return 500;
                    case PieceType::ROOK:
                        return 150;
                    case PieceType::BISHOP:
                    case PieceType::KNIGHT:
                        return 200;
                    default:
                        return 10;
                }
    }

} // namespace board
