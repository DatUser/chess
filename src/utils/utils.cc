#include "utils.hh"

namespace utils {
    long long int two_pow(int power) {
        long long int res = 1;
        for (int i = 0; i < power; i++) {
            res *= 2;
        } return res;
    }
}
