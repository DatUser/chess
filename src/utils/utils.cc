#include "utils.hh"
#include <iostream>

namespace utils {
    unsigned long long int two_pow(int power) {
        unsigned long long int res = 1;
        return res << power;
    }

    int pow_two(unsigned long long int number) {
        if (number == 0) {
            return -1;
        }
        int res = 0;
        while (number > 1) {
            number /= 2;
            res++;
        } return res;
    }

    board::Position to_position(int power) {
        return to_position(power % 8, power / 8);
    }

    board::Position to_position(int x, int y) {
        board::File file;
        board::Rank rank;
        // std::cout << "x: " << x << " y: " << y << "\n"; // Debug purpose
        bool fail = false;
        switch (x) {
            case 0:
                file = board::File::H;
                break;
            case 1:
                file = board::File::G;
                break;
            case 2:
                file = board::File::F;
                break;
            case 3:
                file = board::File::E;
                break;
            case 4:
                file = board::File::D;
                break;
            case 5:
                file = board::File::C;
                break;
            case 6:
                file = board::File::B;
                break;
            case 7:
                file = board::File::A;
                break;
            default:
                file = board::File::A;
                fail = true;
        } switch (y) {
            case 0:
                rank = board::Rank::ONE;
                break;
            case 1:
                rank = board::Rank::TWO;
                break;
            case 2:
                rank = board::Rank::THREE;
                break;
            case 3:
                rank = board::Rank::FOUR;
                break;
            case 4:
                rank = board::Rank::FIVE;
                break;
            case 5:
                rank = board::Rank::SIX;
                break;
            case 6:
                rank = board::Rank::SEVEN;
                break;
            case 7:
                rank = board::Rank::EIGHT;
                break;
            default:
                rank = board::Rank::ONE;
                fail = true;
        }
        if (fail)
            throw std::invalid_argument("File or rank is negative");
        return board::Position(file, rank);
    }

   unsigned long long int to_int(board::Position position) {
        int file = 7 - utils::utype(position.file_get());
        int rank = utils::utype(position.rank_get());
        return file + (rank * 8);
    }

    unsigned long long int floor_two(unsigned long long int number) {
        unsigned long long int power = 1;
        while (number >>= 1)
            power <<= 1;
        return power;
    }


    Position get_position(unsigned long long int board)
    {
        Rank r;
        if (board >= pow(2, 0) and board <= pow(2, 7))
        {
            r = Rank::ONE;
        }
        else if (board >= pow(2, 8) and board <= pow(2, 15))
        {
            r = Rank::TWO;
        }
        else if (board >= pow(2, 16) and board <= pow(2, 23))
        {
            r = Rank::THREE;
        }
        else if (board >= pow(2, 24) and board <= pow(2, 31))
        {
            r = Rank::FOUR;
        }
        else if (board >= pow(2, 32) and board <= pow(2, 39))
        {
            r = Rank::FIVE;
        }
        else if (board >= pow(2, 40) and board <= pow(2, 47))
        {
            r = Rank::SIX;
        }
        else if (board >= pow(2, 48) and board <= pow(2, 55))
        {
            r = Rank::SEVEN;
        }
        else
        {
            r = Rank::EIGHT;
        }

        int file = log2(board);

        //Since File::A == 0
        File f = static_cast<File>(file - 1);

        return Position(f, r);
    }
}
