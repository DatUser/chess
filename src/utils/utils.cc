#include "utils.hh"

namespace utils {
    unsigned long long int two_pow(int power) {
        unsigned long long int res = 1;
        for (int i = 0; i < power; i++) {
            res *= 2;
        } return res;
    }

    int pow_two(unsigned long long int number) {
        int res = 0;
        while (number > 1) {
            number /= 2;
            res++;
        } return res;
    }


    board::Position to_position(int x, int y) {
        board::File file;
        board::Rank rank;

        switch (x) {
            case 0:
                file = board::File::A;
                break;
            case 1:
                file = board::File::A;
                break;
            case 2:
                file = board::File::A;
                break;
            case 3:
                file = board::File::A;
                break;
            case 4:
                file = board::File::A;
                break;
            case 5:
                file = board::File::A;
                break;
            case 6:
                file = board::File::A;
                break;
            case 7:
                file = board::File::A;
                break;
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
        } return board::Position(file, rank);
    }
}
