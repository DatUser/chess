#pragma once

#include "../all.hh"

namespace utils {

    unsigned long long int two_pow(int power);
    int pow_two(unsigned long long int number);

    board::Position to_position(int power);
    board::Position to_position(int x, int y);
    unsigned long long int to_int(board::Position position);
}
