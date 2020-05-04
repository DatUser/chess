#include "bitboard.hh"

namespace board {

    long long int two_pow(int power) {
        long long int res = 1;
        for (int i = 0; i < power; i++) {
            res *= 2;
        } return res;
    }

    Bitboard::Bitboard(PieceType type, Color color)
    : type_(type)
    {
        board_ = 0;
        switch (type)
        {
            case PieceType::KING:
                board_ = two_pow(color == Color::BLACK ? 59 : 3);
                break;
            case PieceType::QUEEN:
                board_ = two_pow(color == Color::BLACK ? 60 : 4);
                break;
            case PieceType::BISHOP:
                board_ = two_pow(color == Color::BLACK ? 61 : 5);
                board_ |= two_pow(color == Color::BLACK ? 58 : 2);
                break;
            case PieceType::KNIGHT:
                board_ = two_pow(color == Color::BLACK ? 62 : 6);
                board_ |= two_pow(color == Color::BLACK ? 57 : 1);
                break;
            case PieceType::ROOK:
                board_ = two_pow(color == Color::BLACK ? 63 : 7);
                board_ |= two_pow(color == Color::BLACK ? 56 : 0);
                break;
            case PieceType::PAWN:
                for (int i = 0; i < 8; i++) {
                    board_ |= two_pow(color == Color::BLACK ? 48 + i : 8 + i);
                }
                break;
            default:
                throw new std::invalid_argument("Invalid piece for bitboard initialisation");
        }
    }

    Bitboard::Bitboard()
    {
        board_ = 0;
    }

    bool Bitboard::move(Position start_pos, Position end_pos)
    {
        /* Int coordinates of the starting position */
        int s_file = 7 - utils::utype(start_pos.file_get());
        int s_rank = utils::utype(start_pos.rank_get());

        /* Int coordinates of the ending position */
        int e_file = 7 - utils::utype(end_pos.file_get());
        int e_rank = utils::utype(end_pos.rank_get());

        /* Corresponding bits */
        long long int s_bit = two_pow(s_file + (s_rank * 8));
        long long int e_bit = two_pow(e_file + (e_rank * 8));


        if ((board_ & s_bit) && !(board_ & e_bit)) {
            board_ ^= s_bit;
            board_ |= e_bit;
            return true;
        } return false;
    }

    void Bitboard::print() {
        long long int temp = board_;
        for (int i = 7; i >= 0; i--)
        {
            std::cout << i + 1 << "|";
            for (long long int j = 7; j >= 0; j--)
            {
                std::cout << ((temp & two_pow(i * 8 + j)) ? "X" : " ")
                          << "|";
            }
            std::cout << "\n";
        }
        std::cout << "  A B C D E F G H\n";
    }
}
