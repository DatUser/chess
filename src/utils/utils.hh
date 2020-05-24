#pragma once

#include "../all.hh"
#include <cmath>

using namespace board;

namespace utils {

    constexpr unsigned long long int two_pow(int power) {
        unsigned long long int res = 1;
        return res << power;
    }

    constexpr int pow_two(unsigned long long int number) {
        if (number == 0) {
            return -1;
        }
        int res = 0;
        while (number > 1) {
            number /= 2;
            res++;
        } return res;
    }

    board::Position to_position(int power);
    board::Position to_position(int x, int y);
    unsigned long long int to_int(board::Position position);

    constexpr unsigned long long int floor_two(unsigned long long int number) {
        unsigned long long int power = 1;
        while (number >>= 1)
            power <<= 1;
        return power;
    }

    Position get_position(unsigned long long int board);
}
