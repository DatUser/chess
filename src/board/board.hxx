#pragma once

    constexpr bool is_occupied(bit& board, const bit b) {
        return board & b;
    }

    constexpr void compute_king_danger(bit& board, int power) {
        if (power - 9 < 63 && power - 9 >= 0)
        {
            board = board | utils::two_pow(power - 9);
        }
        if (power - 8 < 63 && power - 8 >= 0)
        {
            board = board | utils::two_pow(power - 8);
        }
        if (power - 7 < 63 && power - 7 >= 0)
        {
            board = board | utils::two_pow(power - 7);
        }
        if (power - 1 < 63 && power - 1 >= 0)
        {
            board = board | utils::two_pow(power - 1);
        }
        if (power + 1 < 63)
        {
            board = board | utils::two_pow(power + 1);
        }
        if (power + 7 < 63)
        {
            board = board | utils::two_pow(power + 7);
        }
        if (power + 8 < 63)
        {
            board = board | utils::two_pow(power + 8);
        }
        if (power + 9 < 63)
        {
            board = board | utils::two_pow(power + 9);
        }
    }

    constexpr void compute_pawn_danger(bit& board, bit& ally,
            bit& pawns, bool white) {
        unsigned long long int tmp = pawns;
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            // 2^floor_pow = floor
            int floor_pow = utils::pow_two(floor);
            int offset = white ? 9 : -7;
            unsigned long long int res = utils::two_pow(floor_pow + offset);
            if (offset + floor_pow <= 63 and offset + floor_pow >= 0) {
                if (floor_pow % 8 < 7 && !is_occupied(ally, res)) {
                    board = board | res;
                }
            }

            offset = white ? 7 : -9;
            res = utils::two_pow(floor_pow + offset);
            if (offset + floor_pow <= 63 and offset + floor_pow >= 0) {
                if (floor_pow % 8 > 0 && !is_occupied(ally, res)) {
                    board = board | res;
                }
            }
        }
    }

    constexpr void compute_knight_danger(bit& board, bit& ally,
            bit& knights) {
        unsigned long long int tmp = knights;
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            // 2^floor_pow = floor
            int floor_pow = utils::pow_two(floor);
            if (floor_pow  / 8 < 6 && floor_pow % 8 < 7
                    && !is_occupied(ally, utils::two_pow(floor_pow + 17))) {
                board = board
                    | utils::two_pow(floor_pow + 17);
            } if (floor_pow / 8 < 6 && floor_pow % 8 > 0
                    && !is_occupied(ally, utils::two_pow(floor_pow + 15))) {
                board = board
                    | utils::two_pow(floor_pow + 15);
            } if (floor_pow / 8 < 7 && floor_pow % 8 < 6
                    && !is_occupied(ally, utils::two_pow(floor_pow + 10))) {
                board = board
                    | utils::two_pow(floor_pow + 10);
            } if (floor_pow / 8 < 7 && floor_pow % 8 > 1
                    && !is_occupied(ally, utils::two_pow(floor_pow + 6))) {
                board = board
                    | utils::two_pow(floor_pow + 6);
            } if (floor_pow / 8 > 0 && floor_pow % 8 < 6
                    && !is_occupied(ally, utils::two_pow(floor_pow - 6))) {
                board = board
                    | utils::two_pow(floor_pow - 6);
            } if (floor_pow / 8 > 0 && floor_pow % 8 > 1
                    && !is_occupied(ally, utils::two_pow(floor_pow - 10))) {
                board = board
                    | utils::two_pow(floor_pow - 10);
            } if (floor_pow / 8 > 1 && floor_pow % 8 < 7
                    && !is_occupied(ally, utils::two_pow(floor_pow - 15))) {
                board = board
                    | utils::two_pow(floor_pow - 15);
            } if (floor_pow / 8 > 1 && floor_pow % 8 > 0
                    && !is_occupied(ally, utils::two_pow(floor_pow - 17))) {
                board = board
                    | utils::two_pow(floor_pow - 17);
            }
        }
    }

    constexpr void compute_lines(bit& board, bit& ally,
            bit& enemy, int power) {
        int temp = power;
        while (temp < 56) {
            temp += 8;
            board = board | utils::two_pow(temp);
            if (is_occupied(enemy, utils::two_pow(temp))
                    || is_occupied(ally, utils::two_pow(temp)))
                break;
        } temp = power;
        while (temp > 7) {
            temp -= 8;
            board = board | utils::two_pow(temp);
            if (is_occupied(enemy, utils::two_pow(temp))
                    || is_occupied(ally, utils::two_pow(temp)))
                break;
        } temp = power;
        while (temp % 8 < 7) {
            temp += 1;
            board = board | utils::two_pow(temp);
            if (is_occupied(enemy, utils::two_pow(temp))
                    || is_occupied(ally, utils::two_pow(temp)))
                break;
        }   temp = power;
        while (temp % 8 > 0) {
            temp -= 1;
            board = board | utils::two_pow(temp);
            if (is_occupied(enemy, utils::two_pow(temp))
                    || is_occupied(ally, utils::two_pow(temp)))
                break;
        } temp = power;
    }

    constexpr void compute_diagonals(bit& board, bit& ally,
            bit& enemy, int power) {
        int temp = power;
        while (temp / 8 < 7 && temp % 8 < 7) {
            temp += 9;
            board = board | utils::two_pow(temp);
            if (is_occupied(enemy, utils::two_pow(temp)) ||
                    is_occupied(ally, utils::two_pow(temp)))
                break;
        } temp = power;
        while (temp / 8 < 7 && temp % 8 > 0) {
            temp += 7;
            board = board | utils::two_pow(temp);
            if (is_occupied(enemy, utils::two_pow(temp)) ||
                    is_occupied(ally, utils::two_pow(temp)))
                break;
        } temp = power;
        while (temp / 8 > 0 && temp % 8 > 0) {
            temp -= 9;
            board = board | utils::two_pow(temp);
            if (is_occupied(enemy, utils::two_pow(temp)) ||
                    is_occupied(ally, utils::two_pow(temp)))
                break;
        } temp = power;
        while (temp / 8 > 0 && temp % 8 < 7) {
            temp -= 7;
            board = board | utils::two_pow(temp);
            if (is_occupied(enemy, utils::two_pow(temp)) ||
                    is_occupied(ally, utils::two_pow(temp)))
                break;
        }
    }

    constexpr void compute_queen_danger(bit& board, bit& ally,
            bit& enemy, bit& queens) {
        unsigned long long int tmp = queens;
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            compute_lines(board, ally, enemy, utils::pow_two(floor));
            compute_diagonals(board, ally, enemy, utils::pow_two(floor));
        }
    }

    constexpr void compute_bishop_danger(bit& board, bit& ally,
            bit& enemy, bit& bishops) {
        unsigned long long int tmp = bishops;
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            compute_diagonals(board, ally, enemy, utils::pow_two(floor));
        }
    }

    constexpr void compute_rook_danger(bit& board, bit& ally,
            bit& enemy, bit& rooks) {
        unsigned long long int tmp = rooks;
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            compute_lines(board, ally, enemy, utils::pow_two(floor));
        }
    }

    constexpr void move(bit& board, bit s_bit, bit e_bit)
    {
        if ((board & s_bit && !(board & e_bit)))
        {
            board ^= s_bit;
            board |= e_bit;
        }
    }

    constexpr void case_set(bit& board, bit b)
    {
        board |= b;
    }

    constexpr void remove(bit& board, bit b)
    {
        board ^= b;
    }
