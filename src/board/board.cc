#include "board.hh"

#define WHITE_RIGHT_ROOK 1<<0
#define WHITE_LEFT_ROOK 1<<7
#define BLACK_RIGHT_ROOK 1<<55
#define BLACK_LEFT_ROOK 1<<63

namespace board {

    Board::Board()
    {
        king_wb = shared_bit(new Bitboard(PieceType::KING, Color::WHITE));
        queen_wb = shared_bit(new Bitboard(PieceType::QUEEN, Color::WHITE));
        knight_wb = shared_bit(new Bitboard(PieceType::KNIGHT, Color::WHITE));
        bishop_wb = shared_bit(new Bitboard(PieceType::BISHOP, Color::WHITE));
        rook_wb = shared_bit(new Bitboard(PieceType::ROOK, Color::WHITE));
        pawn_wb = shared_bit(new Bitboard(PieceType::PAWN, Color::WHITE));

        king_bb = shared_bit(new Bitboard(PieceType::KING, Color::BLACK));
        queen_bb = shared_bit(new Bitboard(PieceType::QUEEN, Color::BLACK));
        knight_bb = shared_bit(new Bitboard(PieceType::KNIGHT, Color::BLACK));
        bishop_bb = shared_bit(new Bitboard(PieceType::BISHOP, Color::BLACK));
        rook_bb = shared_bit(new Bitboard(PieceType::ROOK, Color::BLACK));
        pawn_bb = shared_bit(new Bitboard(PieceType::PAWN, Color::BLACK));

        occupied_board = shared_bit(new Bitboard());

        white_occupied_board = shared_bit(new Bitboard());
        black_occupied_board = shared_bit(new Bitboard());

        white_danger = shared_bit(new Bitboard());
        black_danger = shared_bit(new Bitboard());
    }

    Board::Board(string ranks)
    {
        king_wb = shared_bit(new Bitboard());
        queen_wb = shared_bit(new Bitboard());
        knight_wb = shared_bit(new Bitboard());
        bishop_wb = shared_bit(new Bitboard());
        rook_wb = shared_bit(new Bitboard());
        pawn_wb = shared_bit(new Bitboard());

        king_bb = shared_bit(new Bitboard());
        queen_bb = shared_bit(new Bitboard());
        knight_bb = shared_bit(new Bitboard());
        bishop_bb = shared_bit(new Bitboard());
        rook_bb = shared_bit(new Bitboard());
        pawn_bb = shared_bit(new Bitboard());

        occupied_board = shared_bit(new Bitboard());

        white_occupied_board = shared_bit(new Bitboard());
        black_occupied_board = shared_bit(new Bitboard());

        white_danger = shared_bit(new Bitboard());
        black_danger = shared_bit(new Bitboard());

        int rank = 7;
        int file = 0;
        for (long unsigned int i = 0; i < ranks.size(); i++)
        {
            if (ranks[i] >= 'a' && ranks[i] <= 'z')
            {
                PieceType piece = char_to_piece(ranks[i]);
                Rank r = (Rank) rank;
                File f = (File) file;
                Position pos = Position(f, r);
                put_piece(Color::BLACK, piece, pos);
                file++;
            }
            else if (ranks[i] >= 'A' && ranks[i] <= 'Z')
            {
                PieceType piece = char_to_piece(ranks[i]);
                Rank r = (Rank) rank;
                File f = (File) file;
                Position pos = Position(f, r);
                put_piece(Color::WHITE, piece, pos);
                file++;
            }
            else if (ranks[i] >= '1' && ranks[i] <= '8')
            {
                int number = ranks[i] - '0';
                file += number;
            }
            else if (ranks[i] == '/')
            {
                rank--;
                file = 0;
            }
        }

    }

    Board::Board(Board& board)
    {
        king_wb = shared_bit(new Bitboard(board.king_wb->board_get()));
        queen_wb = shared_bit(new Bitboard(board.queen_wb->board_get()));
        rook_wb = shared_bit(new Bitboard(board.rook_wb->board_get()));
        knight_wb = shared_bit(new Bitboard(board.knight_wb->board_get()));
        bishop_wb = shared_bit(new Bitboard(board.bishop_wb->board_get()));
        pawn_wb = shared_bit(new Bitboard(board.pawn_wb->board_get()));

        king_bb = shared_bit(new Bitboard(board.king_bb->board_get()));
        queen_bb = shared_bit(new Bitboard(board.queen_bb->board_get()));
        rook_bb = shared_bit(new Bitboard(board.rook_bb->board_get()));
        knight_bb = shared_bit(new Bitboard(board.knight_bb->board_get()));
        bishop_bb = shared_bit(new Bitboard(board.bishop_bb->board_get()));
        pawn_bb = shared_bit(new Bitboard(board.pawn_bb->board_get()));

        white_occupied_board = shared_bit(new Bitboard(
                                    board.white_occupied_board->board_get()));
        black_occupied_board = shared_bit(new Bitboard(
                                    board.black_occupied_board->board_get()));
        occupied_board = shared_bit(new Bitboard(
                                    board.occupied_board->board_get()));

        white_danger = shared_bit(new Bitboard(board.white_danger
                                  ->board_get()));
        black_danger = shared_bit(new Bitboard(board.black_danger
                                  ->board_get()));
    }

    Board& Board::operator=(Board& board)
    {
        king_wb = shared_bit(new Bitboard(board.king_wb->board_get()));
        queen_wb = shared_bit(new Bitboard(board.queen_wb->board_get()));
        rook_wb = shared_bit(new Bitboard(board.rook_wb->board_get()));
        knight_wb = shared_bit(new Bitboard(board.knight_wb->board_get()));
        bishop_wb = shared_bit(new Bitboard(board.bishop_wb->board_get()));
        pawn_wb = shared_bit(new Bitboard(board.pawn_wb->board_get()));

        king_bb = shared_bit(new Bitboard(board.king_bb->board_get()));
        queen_bb = shared_bit(new Bitboard(board.queen_bb->board_get()));
        rook_bb = shared_bit(new Bitboard(board.rook_bb->board_get()));
        knight_bb = shared_bit(new Bitboard(board.knight_bb->board_get()));
        bishop_bb = shared_bit(new Bitboard(board.bishop_bb->board_get()));
        pawn_bb = shared_bit(new Bitboard(board.pawn_bb->board_get()));

        white_occupied_board = shared_bit(new Bitboard(
                                    board.white_occupied_board->board_get()));
        black_occupied_board = shared_bit(new Bitboard(
                                    board.black_occupied_board->board_get()));
        occupied_board = shared_bit(new Bitboard(
                                    board.occupied_board->board_get()));

        white_danger = shared_bit(new Bitboard(board.white_danger
                                  ->board_get()));
        black_danger = shared_bit(new Bitboard(board.black_danger
                                  ->board_get()));
        return *this;
    }

    void Board::put_piece(Color color, PieceType piece, Position pos)
    {
        switch (piece)
        {
            case PieceType::QUEEN:
                (color == Color::WHITE ? queen_wb : queen_bb)->case_set(pos);
                break;
            case PieceType::KING:
                (color == Color::WHITE ? king_wb : king_bb)->case_set(pos);
                break;
            case PieceType::KNIGHT:
                (color == Color::WHITE ? knight_wb : knight_bb)->case_set(pos);
                break;
            case PieceType::BISHOP:
                (color == Color::WHITE ? bishop_wb : bishop_bb)->case_set(pos);
                break;
            case PieceType::ROOK:
                (color == Color::WHITE ? rook_wb : rook_bb)->case_set(pos);
                break;
            default:
                (color == Color::WHITE ? pawn_wb : pawn_bb)->case_set(pos);
        }

    }

    void Board::refresh_occupied()
    {
        white_occupied_board->clear();
        black_occupied_board->clear();
        occupied_board->clear();

        *white_occupied_board |= *king_wb;
        *white_occupied_board |= *queen_wb;
        *white_occupied_board |= *knight_wb;
        *white_occupied_board |= *bishop_wb;
        *white_occupied_board |= *rook_wb;
        *white_occupied_board |= *pawn_wb;

        *black_occupied_board |= *king_bb;
        *black_occupied_board |= *queen_bb;
        *black_occupied_board |= *knight_bb;
        *black_occupied_board |= *bishop_bb;
        *black_occupied_board |= *rook_bb;
        *black_occupied_board |= *pawn_bb;

        *occupied_board |= *white_occupied_board;
        *occupied_board |= *black_occupied_board;
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
        white_danger = shared_bit(new Bitboard());

        int king_power = utils::pow_two(king_bb->board_get());
        compute_king_danger(white_danger, king_power);

        compute_queen_danger(white_danger, black_occupied_board,
                            white_occupied_board, queen_bb);

        compute_bishop_danger(white_danger, black_occupied_board,
                              white_occupied_board, bishop_bb);

        compute_rook_danger(white_danger, black_occupied_board,
                            white_occupied_board, rook_bb);

        compute_pawn_danger(white_danger, black_occupied_board,
                            pawn_bb, false);

        compute_knight_danger(white_danger, black_occupied_board,
                              knight_bb);
    }

    void Board::compute_black_danger() {
        black_danger = shared_bit(new Bitboard());

        int king_power = utils::pow_two(king_wb->board_get());
        compute_king_danger(black_danger, king_power);

        compute_queen_danger(black_danger, white_occupied_board,
                              black_occupied_board, queen_wb);

        compute_bishop_danger(black_danger, white_occupied_board,
                              black_occupied_board, bishop_wb);

        compute_rook_danger(black_danger, white_occupied_board,
                            black_occupied_board, rook_wb);

        compute_pawn_danger(black_danger, white_occupied_board,
                            pawn_wb, true);

        compute_knight_danger(black_danger, white_occupied_board,
                              knight_wb);
    }

    opt_piecetype_t Board::is_occupied(const Position& position, Color color)
        const {
        auto power = utils::two_pow(utils::to_int(position));
        if (color == Color::WHITE) {
            if (pawn_wb->board_get() & power)
                return PieceType::PAWN;
            if (bishop_wb->board_get() & power)
                return PieceType::BISHOP;
            if (knight_wb->board_get() & power)
                return PieceType::KNIGHT;
            if (rook_wb->board_get() & power)
                return PieceType::ROOK;
            if (queen_wb->board_get() & power)
                return PieceType::QUEEN;
            if (king_wb->board_get() & power)
                return PieceType::KING;
        } else if (color == Color::BLACK) {
            if (pawn_bb->board_get() & power)
                return PieceType::PAWN;
            if (bishop_bb->board_get() & power)
                return PieceType::BISHOP;
            if (knight_bb->board_get() & power)
                return PieceType::KNIGHT;
            if (rook_bb->board_get() & power)
                return PieceType::ROOK;
            if (queen_bb->board_get() & power)
                return PieceType::QUEEN;
            if (king_bb->board_get() & power)
                return PieceType::KING;
        }
        return std::optional<PieceType>();
    }

    /*bool Board::is_occupied(Position position) {
        auto power = utils::to_int(position);
        return occupied_board->board_get() & utils::two_pow(power);
    }*/

    bool Board::is_occupied(shared_bit board, const Position& position) const {
        auto power = utils::to_int(position);
        return board->board_get() & utils::two_pow(power);
    }

    void Board::do_move(Move& move, Color color) {
        if (move.king_castling_get())
        {
            auto board = (color == Color::WHITE ? king_wb : king_bb);
            board->move(move.move_get().first, move.move_get().second);
            auto board2 = (color == Color::WHITE ? rook_wb : rook_bb);
            Rank rank = (color == Color::WHITE ? Rank::ONE : Rank::EIGHT);
            board2->move(Position(File::H, rank), Position(File::F, rank));
        }
        else if (move.queen_castling_get())
        {
            auto board = (color == Color::WHITE ? king_wb : king_bb);
            board->move(move.move_get().first, move.move_get().second);
            auto board2 = (color == Color::WHITE ? rook_wb : rook_bb);
            Rank rank = (color == Color::WHITE ? Rank::ONE : Rank::EIGHT);
            board2->move(Position(File::A, rank), Position(File::D, rank));
        }
        else
        {
            shared_bit board;
            switch (move.piece_get())
            {
                case PieceType::QUEEN:
                    board = (color == Color::WHITE ? queen_wb : queen_bb);
                    break;
                case PieceType::ROOK:
                    board = (color == Color::WHITE ? rook_wb : rook_bb);
                    break;
                case PieceType::BISHOP:
                    board =  (color == Color::WHITE ? bishop_wb : bishop_bb);
                    break;
                case PieceType::KNIGHT:
                    board = (color == Color::WHITE ? knight_wb : knight_bb);
                    break;
                case PieceType::PAWN:
                    board = (color == Color::WHITE ? pawn_wb : pawn_bb);
                    break;
                case PieceType::KING:
                    board = (color == Color::WHITE ? king_wb : king_bb);
                    break;
                default:
                    break;
                    //board = new Bitboard();
            }
            board->move(move.move_get().first, move.move_get().second);
            if (move.capture_get() != PieceType::NONE)
            {
                switch (move.capture_get())
                {
                    case PieceType::QUEEN:
                        board = (color == Color::WHITE ? queen_bb : queen_wb);
                        break;
                    case PieceType::ROOK:
                        board = (color == Color::WHITE ? rook_bb : rook_wb);
                        break;
                    case PieceType::BISHOP:
                        board =  (color == Color::WHITE ? bishop_bb : bishop_wb);
                        break;
                    case PieceType::KNIGHT:
                        board = (color == Color::WHITE ? knight_bb : knight_wb);
                        break;
                    case PieceType::PAWN:
                        board = (color == Color::WHITE ? pawn_bb : pawn_wb);
                        break;
                    case PieceType::KING:
                        board = (color == Color::WHITE ? king_bb : king_wb);
                        break;
                    default:
                        break;

                }
                board->remove(move.move_get().second);
            }
            if (move.promotion_get() != PieceType::NONE)
            {
                board = (color == Color::WHITE ? pawn_wb : pawn_bb);
                board->remove(move.move_get().second);
                switch (move.promotion_get())
                {
                    case PieceType::QUEEN:
                        board = (color == Color::WHITE ? queen_wb: queen_bb);
                        break;
                    case PieceType::ROOK:
                        board = (color == Color::WHITE ? rook_wb : rook_bb);
                        break;
                    case PieceType::BISHOP:
                        board =  (color == Color::WHITE ? bishop_wb : bishop_bb);
                        break;
                    case PieceType::KNIGHT:
                        board = (color == Color::WHITE ? knight_wb : knight_bb);
                        break;
                    default:
                        break;
                }
                board->case_set(move.move_get().second);
            }
        }
    }

    bool Board::is_check(bool white_turn_)
    {
        compute_danger();
        if (white_turn_)
            return white_danger->board_get() & king_wb->board_get();
        return black_danger->board_get() & king_bb->board_get();
    }

    std::vector<Position> Board::get_white_king() {
        std::vector<Position> res = std::vector<Position>();
        unsigned long long int tmp = king_wb->board_get();
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            int floor_pow = utils::pow_two(floor);
            res.emplace_back(utils::to_position(floor_pow));
        } return res;
    }

    std::vector<Position> Board::get_white_queen() {
        std::vector<Position> res = std::vector<Position>();
        unsigned long long int tmp = queen_wb->board_get();
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            int floor_pow = utils::pow_two(floor);
            res.emplace_back(utils::to_position(floor_pow));
        } return res;
    }


    std::vector<Position> Board::get_white_knight() {
        std::vector<Position> res = std::vector<Position>();
        unsigned long long int tmp = knight_wb->board_get();
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            int floor_pow = utils::pow_two(floor);
            res.emplace_back(utils::to_position(floor_pow));
        } return res;
    }


    std::vector<Position> Board::get_white_bishop() {
        std::vector<Position> res = std::vector<Position>();
        unsigned long long int tmp = bishop_wb->board_get();
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            int floor_pow = utils::pow_two(floor);
            res.emplace_back(utils::to_position(floor_pow));
        } return res;
    }


    std::vector<Position> Board::get_white_rook() {
        std::vector<Position> res = std::vector<Position>();
        unsigned long long int tmp = rook_wb->board_get();
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            int floor_pow = utils::pow_two(floor);
            res.emplace_back(utils::to_position(floor_pow));
        } return res;
    }


    std::vector<Position> Board::get_white_pawn() {
        std::vector<Position> res = std::vector<Position>();
        unsigned long long int tmp = pawn_wb->board_get();
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            int floor_pow = utils::pow_two(floor);
            res.emplace_back(utils::to_position(floor_pow));
        } return res;
    }


    std::vector<Position> Board::get_black_king() {
        std::vector<Position> res = std::vector<Position>();
        unsigned long long int tmp = king_bb->board_get();
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            int floor_pow = utils::pow_two(floor);
            res.emplace_back(utils::to_position(floor_pow));
        } return res;
    }

    std::vector<Position> Board::get_black_queen() {
        std::vector<Position> res = std::vector<Position>();
        unsigned long long int tmp = queen_bb->board_get();
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            int floor_pow = utils::pow_two(floor);
            res.emplace_back(utils::to_position(floor_pow));
        } return res;
    }


    std::vector<Position> Board::get_black_knight() {
        std::vector<Position> res = std::vector<Position>();
        unsigned long long int tmp = knight_bb->board_get();
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            int floor_pow = utils::pow_two(floor);
            res.emplace_back(utils::to_position(floor_pow));
        } return res;
    }


    std::vector<Position> Board::get_black_bishop() {
        std::vector<Position> res = std::vector<Position>();
        unsigned long long int tmp = bishop_bb->board_get();
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            int floor_pow = utils::pow_two(floor);
            res.emplace_back(utils::to_position(floor_pow));
        } return res;
    }


    std::vector<Position> Board::get_black_rook() {
        std::vector<Position> res = std::vector<Position>();
        unsigned long long int tmp = rook_bb->board_get();
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            int floor_pow = utils::pow_two(floor);
            res.emplace_back(utils::to_position(floor_pow));
        } return res;
    }


    std::vector<Position> Board::get_black_pawn() {
        std::vector<Position> res = std::vector<Position>();
        unsigned long long int tmp = pawn_bb->board_get();
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            int floor_pow = utils::pow_two(floor);
            res.emplace_back(utils::to_position(floor_pow));
        } return res;
    }


    void Board::compute_king_danger(shared_bit board, int power) {
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

    void Board::compute_queen_danger(shared_bit board, shared_bit ally,
                                     shared_bit enemy, shared_bit queens) {
        unsigned long long int tmp = queens->board_get();
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            compute_lines(board, ally, enemy, utils::pow_two(floor));
            compute_diagonals(board, ally, enemy, utils::pow_two(floor));
        }
    }

    void Board::compute_bishop_danger(shared_bit board, shared_bit ally,
                                      shared_bit enemy, shared_bit bishops) {
        unsigned long long int tmp = bishops->board_get();
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            compute_diagonals(board, ally, enemy, utils::pow_two(floor));
        }
    }

    void Board::compute_rook_danger(shared_bit board, shared_bit ally,
                                    shared_bit enemy, shared_bit rooks) {
        unsigned long long int tmp = rooks->board_get();
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            compute_lines(board, ally, enemy, utils::pow_two(floor));
        }
    }

    void Board::compute_pawn_danger(shared_bit board, shared_bit ally,
                                    shared_bit pawns, bool white) {
        unsigned long long int tmp = pawns->board_get();
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            // 2^floor_pow = floor
            int floor_pow = utils::pow_two(floor);
            int offset = white ? 9 : -7;
            unsigned long long int res = utils::two_pow(floor_pow + offset);
            if (offset + floor_pow <= 63 and offset + floor_pow >= 0) {
                Position position = utils::to_position(floor_pow + offset);
                if (floor_pow % 8 < 7 && !is_occupied(ally, position)) {
                    board->board_set(board->board_get() | res);
                }
            }
            /*offset = white ? 8 : -8;
            res = utils::two_pow(floor_pow + offset);
            if (offset + floor_pow <= 63 and offset + floor_pow >= 0) {
                Position position = utils::to_position(floor_pow + offset);
                if (!is_occupied(ally, position)) {
                    board->board_set(board->board_get() | res);
                }
            }*/
            offset = white ? 7 : -9;
            res = utils::two_pow(floor_pow + offset);
            if (offset + floor_pow <= 63 and offset + floor_pow >= 0) {
                Position position = utils::to_position(floor_pow + offset);
                if (floor_pow % 8 > 0 && !is_occupied(ally, position)) {
                    board->board_set(board->board_get() | res);
                }
            }
        }
    }

    void Board::compute_knight_danger(shared_bit board, shared_bit ally,
                                      shared_bit knights) {
        unsigned long long int tmp = knights->board_get();
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            // 2^floor_pow = floor
            int floor_pow = utils::pow_two(floor);
            if (floor_pow  / 8 < 6 && floor_pow % 8 < 7
                && !is_occupied(ally, utils::to_position(floor_pow + 17))) {
                board->board_set(board->board_get()
                | utils::two_pow(floor_pow + 17));
            } if (floor_pow / 8 < 6 && floor_pow % 8 > 0
                  && !is_occupied(ally, utils::to_position(floor_pow + 15))) {
                board->board_set(board->board_get()
                | utils::two_pow(floor_pow + 15));
            } if (floor_pow / 8 < 7 && floor_pow % 8 < 6
                  && !is_occupied(ally, utils::to_position(floor_pow + 10))) {
                board->board_set(board->board_get()
                | utils::two_pow(floor_pow + 10));
            } if (floor_pow / 8 < 7 && floor_pow % 8 > 1
                  && !is_occupied(ally, utils::to_position(floor_pow + 6))) {
                board->board_set(board->board_get()
                | utils::two_pow(floor_pow + 6));
            } if (floor_pow / 8 > 0 && floor_pow % 8 < 6
                  && !is_occupied(ally, utils::to_position(floor_pow - 6))) {
                board->board_set(board->board_get()
                | utils::two_pow(floor_pow - 6));
            } if (floor_pow / 8 > 0 && floor_pow % 8 > 1
                  && !is_occupied(ally, utils::to_position(floor_pow - 10))) {
                board->board_set(board->board_get()
                | utils::two_pow(floor_pow - 10));
            } if (floor_pow / 8 > 1 && floor_pow % 8 < 7
                  && !is_occupied(ally, utils::to_position(floor_pow - 15))) {
                board->board_set(board->board_get()
                | utils::two_pow(floor_pow - 15));
            } if (floor_pow / 8 > 1 && floor_pow % 8 > 0
                  && !is_occupied(ally, utils::to_position(floor_pow - 17))) {
                board->board_set(board->board_get()
                | utils::two_pow(floor_pow - 17));
            }
        }
    }

    void Board::compute_lines(shared_bit board, shared_bit ally,
                       shared_bit enemy, int power) {
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

    void Board::compute_diagonals(shared_bit board, shared_bit ally,
                          shared_bit enemy, int power) {
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

    void Board::print()
    {
        for (int i = 7; i >= 0; i--)
        {
            std::cout << i + 1 << "|";
            for (int j = 7; j >= 0; j--)
            {
                long long int temp = utils::two_pow(i * 8 + j);
                if (king_wb->board_get() & temp)
                    std::cout << "K";
                else if (queen_wb->board_get() & temp)
                    std::cout << "Q";
                else if (knight_wb->board_get() & temp)
                    std::cout << "N";
                else if (bishop_wb->board_get() & temp)
                    std::cout << "B";
                else if (rook_wb->board_get() & temp)
                    std::cout << "R";
                else if (pawn_wb->board_get() & temp)
                    std::cout << "P";
                else if (king_bb->board_get() & temp)
                    std::cout << "k";
                else if (queen_bb->board_get() & temp)
                    std::cout << "q";
                else if (knight_bb->board_get() & temp)
                    std::cout << "n";
                else if (bishop_bb->board_get() & temp)
                    std::cout << "b";
                else if (rook_bb->board_get() & temp)
                    std::cout << "r";
                else if (pawn_bb->board_get() & temp)
                    std::cout << "p";
                else
                    std::cout << " ";
                std::cout << "|";
            }
            std::cout << "\n";
        }
        std::cout << "  A B C D E F G H\n";
    }
}
