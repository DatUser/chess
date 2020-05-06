#include "board.hh"

namespace board {

    Board::Board()
    {
        king_wb = Bitboard(PieceType::KING, Color::WHITE);
        queen_wb = Bitboard(PieceType::QUEEN, Color::WHITE);
        knight_wb = Bitboard(PieceType::KNIGHT, Color::WHITE);
        bishop_wb = Bitboard(PieceType::BISHOP, Color::WHITE);
        rook_wb = Bitboard(PieceType::ROOK, Color::WHITE);
        pawn_wb = Bitboard(PieceType::PAWN, Color::WHITE);

        king_bb = Bitboard(PieceType::KING, Color::BLACK);
        queen_bb = Bitboard(PieceType::QUEEN, Color::BLACK);
        knight_bb = Bitboard(PieceType::KNIGHT, Color::BLACK);
        bishop_bb = Bitboard(PieceType::BISHOP, Color::BLACK);
        rook_bb = Bitboard(PieceType::ROOK, Color::BLACK);
        pawn_bb = Bitboard(PieceType::PAWN, Color::BLACK);

        occupied_board = Bitboard();

        //white_danger = Bitboard();
        //black_danger = Bitboard();
    }

    void Board::refresh_occupied()
    {
        white_occupied_board |= king_wb;
        white_occupied_board |= queen_wb;
        white_occupied_board |= knight_wb;
        white_occupied_board |= bishop_wb;
        white_occupied_board |= rook_wb;
        white_occupied_board |= pawn_wb;

        black_occupied_board |= king_bb;
        black_occupied_board |= queen_bb;
        black_occupied_board |= knight_bb;
        black_occupied_board |= bishop_bb;
        black_occupied_board |= rook_bb;
        black_occupied_board |= pawn_bb;

        occupied_board |= white_occupied_board;
        occupied_board |= black_occupied_board;
        /* white_occupied_board.print();
        black_occupied_board.print();
        occupied_board.print(); */// Debug purpose
    }

    void Board::compute_danger() {
        // First refresh the occupied bitboard
        refresh_occupied();
        // Then computes the white danger bitboard
        compute_white_danger();
        // DEBUG PURPOSES
        /* white_danger.print();
        black_occupied_board = Bitboard();
        compute_white_danger();
        white_danger.print();*/
        // END OF DEBUG PURPOSES
        // And do the same with the black danger bitboard
        compute_black_danger();
        //black_danger.print();
        //white_occupied_board = Bitboard();
        //compute_black_danger();
        //black_danger.print();
    }

    void Board::compute_white_danger() {
        white_danger = Bitboard();

        int king_power = utils::pow_two(king_bb.board_get());
        compute_king_danger(&white_danger, king_power);

        int queen_power = utils::pow_two(queen_bb.board_get());
        if (queen_power != -1)
            compute_queen_danger(&white_danger, black_occupied_board,
                                white_occupied_board, queen_power);

        compute_bishop_danger(&white_danger, black_occupied_board,
                              white_occupied_board, bishop_bb);

        compute_rook_danger(&white_danger, black_occupied_board,
                            white_occupied_board, rook_bb);

        compute_pawn_danger(&white_danger, black_occupied_board,
                            pawn_bb, false);

        compute_knight_danger(&white_danger, black_occupied_board,
                              knight_bb);
    }

    void Board::compute_black_danger() {
        black_danger = Bitboard();

        int king_power = utils::pow_two(king_wb.board_get());
        compute_king_danger(&black_danger, king_power);

        int queen_power = utils::pow_two(queen_wb.board_get());
        if (queen_power != -1)
            compute_queen_danger(&black_danger, white_occupied_board,
                                black_occupied_board, queen_power);

        compute_bishop_danger(&black_danger, white_occupied_board,
                              black_occupied_board, bishop_wb);

        compute_rook_danger(&black_danger, white_occupied_board,
                            black_occupied_board, rook_wb);

        compute_pawn_danger(&black_danger, white_occupied_board,
                            pawn_wb, true);

        compute_knight_danger(&black_danger, white_occupied_board,
                              knight_wb);
    }

    bool Board::is_occupied(Position position) {
        auto power = utils::to_int(position);
        return occupied_board.board_get() & utils::two_pow(power);
    }

    bool Board::is_occupied(Bitboard board, Position position) {
        auto power = utils::to_int(position);
        return board.board_get() & utils::two_pow(power);
    }

    void Board::compute_king_danger(Bitboard* board, int power) {
        if (power - 9 < 63 && power - 9 >= 0)
        {
            board->board_set(board->board_get() | utils::two_pow(power - 9));
        }
        if (power - 8 < 63 && power - 8 >= 0)
        {
            board->board_set(board->board_get() | utils::two_pow(power - 8));
        }
        if (power - 7 < 63 && power - 7 >= 0)
        {
            board->board_set(board->board_get() | utils::two_pow(power - 7));
        }
        if (power - 1 < 63 && power - 1 >= 0)
        {
            board->board_set(board->board_get() | utils::two_pow(power - 1));
        }
        if (power + 1 < 63)
        {
            board->board_set(board->board_get() | utils::two_pow(power + 1));
        }
        if (power + 7 < 63)
        {
            board->board_set(board->board_get() | utils::two_pow(power + 7));
        }
        if (power + 8 < 63)
        {
            board->board_set(board->board_get() | utils::two_pow(power + 8));
        }
        if (power + 9 < 63)
        {
            board->board_set(board->board_get() | utils::two_pow(power + 9));
        }
    }

    void Board::compute_queen_danger(Bitboard* board, Bitboard ally,
                                     Bitboard enemy, int power) {
        compute_lines(board, ally, enemy, power);
        compute_diagonals(board, ally, enemy, power);
    }

    void Board::compute_bishop_danger(Bitboard* board, Bitboard ally,
                                      Bitboard enemy, Bitboard bishops) {
        unsigned long long int tmp = bishops.board_get();
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            compute_diagonals(board, ally, enemy, utils::pow_two(floor));
        }
    }

    void Board::compute_rook_danger(Bitboard* board, Bitboard ally,
                                    Bitboard enemy, Bitboard rooks) {
        unsigned long long int tmp = rooks.board_get();
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            compute_lines(board, ally, enemy, utils::pow_two(floor));
        }
    }

    void Board::compute_pawn_danger(Bitboard *board, Bitboard ally,
                                    Bitboard pawns, bool white) {
        unsigned long long int tmp = pawns.board_get();
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            // 2^floor_pow = floor
            int floor_pow = utils::pow_two(floor);
            int offset = white ? 9 : -7;
            unsigned long long int res = utils::two_pow(floor_pow + offset);
            Position position = utils::to_position(floor_pow + offset);
            if (floor_pow % 8 < 7 && !is_occupied(ally, position)) {
                board->board_set(board->board_get() | res);
            }
            offset = white ? 8 : -8;
            res = utils::two_pow(floor_pow + offset);
            position = utils::to_position(floor_pow + offset);
            if (!is_occupied(ally, position)) {
                board->board_set(board->board_get() | res);
            }
            offset = white ? 7 : -9;
            res = utils::two_pow(floor_pow + offset);
            position = utils::to_position(floor_pow + offset);
            if (floor_pow % 8 > 0 && !is_occupied(ally, position)) {
                board->board_set(board->board_get() | res);
            }
        }
    }

    void Board::compute_knight_danger(Bitboard *board, Bitboard ally,
                                      Bitboard knights) {
        unsigned long long int tmp = knights.board_get();
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            // 2^floor_pow = floor
            int floor_pow = utils::pow_two(floor);
            if (floor_pow  / 8 < 6 && floor_pow % 8 < 7
                && !is_occupied(ally, utils::to_position(floor_pow + 17))) {
                board->board_set(board->board_get() | utils::two_pow(floor_pow + 17));
            } if (floor_pow / 8 < 6 && floor_pow % 8 > 0
                  && !is_occupied(ally, utils::to_position(floor_pow + 15))) {
                board->board_set(board->board_get() | utils::two_pow(floor_pow + 15));
            } if (floor_pow / 8 < 7 && floor_pow % 8 < 6
                  && !is_occupied(ally, utils::to_position(floor_pow + 10))) {
                board->board_set(board->board_get() | utils::two_pow(floor_pow + 10));
            } if (floor_pow / 8 < 7 && floor_pow % 8 > 1
                  && !is_occupied(ally, utils::to_position(floor_pow + 6))) {
                board->board_set(board->board_get() | utils::two_pow(floor_pow + 6));
            } if (floor_pow / 8 > 0 && floor_pow % 8 < 6
                  && !is_occupied(ally, utils::to_position(floor_pow - 6))) {
                board->board_set(board->board_get() | utils::two_pow(floor_pow - 6));
            } if (floor_pow / 8 > 0 && floor_pow % 8 > 1
                  && !is_occupied(ally, utils::to_position(floor_pow - 10))) {
                board->board_set(board->board_get() | utils::two_pow(floor_pow - 10));
            } if (floor_pow / 8 > 1 && floor_pow % 8 < 7
                  && !is_occupied(ally, utils::to_position(floor_pow - 15))) {
                board->board_set(board->board_get() | utils::two_pow(floor_pow - 15));
            } if (floor_pow / 8 > 1 && floor_pow % 8 > 0
                  && !is_occupied(ally, utils::to_position(floor_pow - 17))) {
                board->board_set(board->board_get() | utils::two_pow(floor_pow - 17));
            }
        }
    }

    void Board::compute_lines(Bitboard* board, Bitboard ally,
                       Bitboard enemy, int power) {
        int temp = power;
        while (temp < 56) {
            temp += 8;
            board->board_set(board->board_get() | utils::two_pow(temp));
            if (is_occupied(enemy, utils::to_position(temp))
                || is_occupied(ally, utils::to_position(temp)))
                break;
        } temp = power;
        while (temp > 7) {
            temp -= 8;
            board->board_set(board->board_get() | utils::two_pow(temp));
            if (is_occupied(enemy, utils::to_position(temp))
                || is_occupied(ally, utils::to_position(temp)))
                break;
        } temp = power;
        while (temp % 8 < 7) {
            temp += 1;
            board->board_set(board->board_get() | utils::two_pow(temp));
            if (is_occupied(enemy, utils::to_position(temp))
                || is_occupied(ally, utils::to_position(temp)))
                break;
        }   temp = power;
        while (temp % 8 > 0) {
            temp -= 1;
            board->board_set(board->board_get() | utils::two_pow(temp));
            if (is_occupied(enemy, utils::to_position(temp))
                || is_occupied(ally, utils::to_position(temp)))
                break;
        } temp = power;
    }

    void Board::compute_diagonals(Bitboard *board, Bitboard ally,
                          Bitboard enemy, int power) {
        int temp = power;
        while (temp / 8 < 7 && temp % 8 < 7) {
            temp += 9;
            board->board_set(board->board_get() | utils::two_pow(temp));
            if (is_occupied(enemy, utils::to_position(temp)) ||
                is_occupied(ally, utils::to_position(temp)))
                break;
        } temp = power;
        while (temp / 8 < 7 && temp % 8 > 0) {
            temp += 7;
            board->board_set(board->board_get() | utils::two_pow(temp));
            if (is_occupied(enemy, utils::to_position(temp)) ||
                is_occupied(ally, utils::to_position(temp)))
                break;
        } temp = power;
        while (temp / 8 > 0 && temp % 8 > 0) {
            temp -= 9;
            board->board_set(board->board_get() | utils::two_pow(temp));
            if (is_occupied(enemy, utils::to_position(temp)) ||
                is_occupied(ally, utils::to_position(temp)))
                break;
        } temp = power;
        while (temp / 8 > 0 && temp % 8 < 7) {
            temp -= 7;
            board->board_set(board->board_get() | utils::two_pow(temp));
            if (is_occupied(enemy, utils::to_position(temp)) ||
                is_occupied(ally, utils::to_position(temp)))
                break;
        }
    }
}
