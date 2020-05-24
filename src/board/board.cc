#include "board.hh"

#define WHITE_RIGHT_ROOK 1<<0
#define WHITE_LEFT_ROOK 1<<7
#define BLACK_RIGHT_ROOK 1<<55
#define BLACK_LEFT_ROOK 1<<63

#define BLACK_BASE_PAWN 71776119061217280
#define BLACK_BASE_KNIGHT 4755801206503243776
#define BLACK_BASE_BISHOP 2594073385365405696
#define BLACK_BASE_QUEEN 1152921504606846976
#define BLACK_BASE_KING 576460752303423488

#define WHITE_BASE_PAWN 65280
#define WHITE_BASE_ROOK 129
#define WHITE_BASE_KNIGHT 66
#define WHITE_BASE_BISHOP 36
#define WHITE_BASE_QUEEN 16
#define WHITE_BASE_KING 8

namespace board {

    Board::Board()
    {
        king_wb = WHITE_BASE_KING;
        queen_wb = WHITE_BASE_QUEEN;
        knight_wb = WHITE_BASE_KNIGHT;
        bishop_wb = WHITE_BASE_BISHOP;
        rook_wb = WHITE_BASE_ROOK;
        pawn_wb = WHITE_BASE_PAWN;

        king_bb = BLACK_BASE_KING;
        queen_bb = BLACK_BASE_QUEEN;
        knight_bb = BLACK_BASE_KNIGHT;
        bishop_bb = BLACK_BASE_BISHOP;
        rook_bb = (((unsigned long long int) 1) << 63
                        | ((unsigned long long int) 1) << 55);
        pawn_bb = BLACK_BASE_PAWN;

        occupied_board = 0;

        white_occupied_board = 0;
        black_occupied_board = 0;

        white_danger = 0;
        black_danger = 0;
    }

    Board::Board(string ranks)
    {
        king_wb = 0;
        queen_wb = 0;
        knight_wb = 0;
        bishop_wb = 0;
        rook_wb = 0;
        pawn_wb = 0;

        king_bb = 0;
        queen_bb = 0;
        knight_bb = 0;
        bishop_bb = 0;
        rook_bb = 0;
        pawn_bb = 0;

        occupied_board = 0;

        white_occupied_board = 0;
        black_occupied_board = 0;

        white_danger = 0;
        black_danger = 0;

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
        king_wb = board.king_wb;
        queen_wb = board.queen_wb;
        rook_wb = board.rook_wb;
        knight_wb = board.knight_wb;
        bishop_wb = board.bishop_wb;
        pawn_wb = board.pawn_wb;

        king_bb = board.king_bb;
        queen_bb = board.queen_bb;
        rook_bb = board.rook_bb;
        knight_bb = board.knight_bb;
        bishop_bb = board.bishop_bb;
        pawn_bb = board.pawn_bb;

        white_occupied_board = board.white_occupied_board;
        black_occupied_board = board.black_occupied_board;
        occupied_board = board.occupied_board;

        white_danger = board.white_danger;
        black_danger = board.black_danger;
    }

    Board& Board::operator=(Board& board)
    {
        king_wb = board.king_wb;
        queen_wb = board.queen_wb;
        rook_wb = board.rook_wb;
        knight_wb = board.knight_wb;
        bishop_wb = board.bishop_wb;
        pawn_wb = board.pawn_wb;

        king_bb = board.king_bb;
        queen_bb = board.queen_bb;
        rook_bb = board.rook_bb;
        knight_bb = board.knight_bb;
        bishop_bb = board.bishop_bb;
        pawn_bb = board.pawn_bb;

        white_occupied_board = board.white_occupied_board;
        black_occupied_board = board.black_occupied_board;
        occupied_board = board.occupied_board;

        white_danger = board.white_danger;
        black_danger = board.black_danger;

        return *this;
    }

    void Board::put_piece(Color color, PieceType piece, Position pos)
    {
        switch (piece)
        {
            case PieceType::QUEEN:
                (color == Color::WHITE ? queen_wb : queen_bb)
                    |= two_pow(to_int(pos));
                break;
            case PieceType::KING:
                (color == Color::WHITE ? king_wb : king_bb)
                    |= two_pow(to_int(pos));
                break;
            case PieceType::KNIGHT:
                (color == Color::WHITE ? knight_wb : knight_bb)
                    |= two_pow(to_int(pos));
                break;
            case PieceType::BISHOP:
                (color == Color::WHITE ? bishop_wb : bishop_bb)
                    |= two_pow(to_int(pos));
                break;
            case PieceType::ROOK:
                (color == Color::WHITE ? rook_wb : rook_bb)
                    |= two_pow(to_int(pos));
                break;
            default:
                (color == Color::WHITE ? pawn_wb : pawn_bb)
                    |= two_pow(to_int(pos));
        }

    }

    void Board::refresh_occupied()
    {
        white_occupied_board = 0;
        black_occupied_board = 0;
        occupied_board = 0;

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
    }

    void Board::compute_danger() {
        // First refresh the occupied bitboard
        refresh_occupied();

        // Then computes the white danger bitboard
        compute_white_danger();

        // And do the same with the black danger bitboard
        compute_black_danger();
    }

    void Board::compute_white_danger() {
        white_danger = 0;
        int king_power = utils::pow_two(king_bb);
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
        black_danger = 0;
        int king_power = utils::pow_two(king_wb);
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
            if (pawn_wb & power)
                return PieceType::PAWN;
            if (bishop_wb & power)
                return PieceType::BISHOP;
            if (knight_wb & power)
                return PieceType::KNIGHT;
            if (rook_wb & power)
                return PieceType::ROOK;
            if (queen_wb & power)
                return PieceType::QUEEN;
            if (king_wb & power)
                return PieceType::KING;
        } else if (color == Color::BLACK) {
            if (pawn_bb & power)
                return PieceType::PAWN;
            if (bishop_bb & power)
                return PieceType::BISHOP;
            if (knight_bb & power)
                return PieceType::KNIGHT;
            if (rook_bb & power)
                return PieceType::ROOK;
            if (queen_bb & power)
                return PieceType::QUEEN;
            if (king_bb & power)
                return PieceType::KING;
        }
        return std::optional<PieceType>();
    }


    bool Board::is_occupied(bit& board, const Position& position) const {
        auto power = utils::to_int(position);
        return board & utils::two_pow(power);
    }

    void Board::do_move(Move& move, Color color) {
        Position start_pos = move.move_get().first;
        Position end_pos = move.move_get().second;
        int s_file = 7 - utils::utype(start_pos.file_get());
        int s_rank = utils::utype(start_pos.rank_get());

        int e_file = 7 - utils::utype(end_pos.file_get());
        int e_rank = utils::utype(end_pos.rank_get());

        unsigned long long int s_bit = utils::two_pow(s_file + (s_rank * 8));
        unsigned long long int e_bit = utils::two_pow(e_file + (e_rank * 8));

        if (move.king_castling_get())
        {
            // KING MOVE
            this->move((color == Color::WHITE ? king_wb : king_bb),
                    s_bit, e_bit);

            Rank rank = (color == Color::WHITE ? Rank::ONE : Rank::EIGHT);
            // ROOK MOVE
            this->move((color == Color::WHITE ? rook_wb : rook_bb),
                    utils::two_pow(utils::utype(rank) * 8),
                    utils::two_pow(3 + utils::utype(rank) * 8));
        }
        else if (move.queen_castling_get())
        {
            // KING MOVE
            this->move((color == Color::WHITE ? king_wb : king_bb),
                        s_bit, e_bit);

            Rank rank = (color == Color::WHITE ? Rank::ONE : Rank::EIGHT);
            // ROOK MOVE
            this->move((color == Color::WHITE ? rook_wb : rook_bb),
                        utils::two_pow(7 + utils::utype(rank) * 8),
                        utils::two_pow(4 + utils::utype(rank) * 8));
        }
        else
        {
            switch (move.piece_get())
            {
                case PieceType::QUEEN:
                    this->move((color == Color::WHITE ? queen_wb : queen_bb),
                            s_bit, e_bit);
                    break;
                case PieceType::ROOK:
                    this->move((color == Color::WHITE ? rook_wb : rook_bb),
                            s_bit, e_bit);
                    break;
                case PieceType::BISHOP:
                    this->move((color == Color::WHITE ? bishop_wb : bishop_bb),
                            s_bit, e_bit);
                    break;
                case PieceType::KNIGHT:
                    this->move((color == Color::WHITE ? knight_wb : knight_bb),
                            s_bit, e_bit);
                    break;
                case PieceType::PAWN:
                    this->move((color == Color::WHITE ? pawn_wb : pawn_bb),
                            s_bit, e_bit);
                    break;
                case PieceType::KING:
                    this->move((color == Color::WHITE ? king_wb : king_bb),
                            s_bit, e_bit);
                    break;
                default:
                    break;
            }
            if (move.capture_get() != PieceType::NONE)
            {
                switch (move.capture_get())
                {
                    case PieceType::QUEEN:
                        (color == Color::WHITE ? queen_bb : queen_wb) ^= e_bit;
                        break;
                    case PieceType::ROOK:
                        (color == Color::WHITE ? rook_bb : rook_wb) ^= e_bit;
                        break;
                    case PieceType::BISHOP:
                         (color == Color::WHITE ? bishop_bb : bishop_wb)
                                        ^= e_bit;
                        break;
                    case PieceType::KNIGHT:
                        (color == Color::WHITE ? knight_bb : knight_wb)
                                        ^= e_bit;
                        break;
                    case PieceType::PAWN:
                        (color == Color::WHITE ? pawn_bb : pawn_wb) ^= e_bit;
                        break;
                    case PieceType::KING:
                        (color == Color::WHITE ? king_bb : king_wb) ^= e_bit;
                        break;
                    default:
                        break;

                }
            }
            if (move.promotion_get() != PieceType::NONE)
            {
                remove((color == Color::WHITE ? pawn_wb : pawn_bb), e_bit);

                switch (move.promotion_get())
                {
                    case PieceType::QUEEN:
                        case_set((color == Color::WHITE ? queen_wb: queen_bb)
                            , e_bit);
                        break;
                    case PieceType::ROOK:
                        case_set((color == Color::WHITE ? rook_wb : rook_bb)
                            , e_bit);
                        break;
                    case PieceType::BISHOP:
                        case_set((color == Color::WHITE ? bishop_wb
                            : bishop_bb) , e_bit);
                        break;
                    case PieceType::KNIGHT:
                        case_set((color == Color::WHITE ? knight_wb
                            : knight_bb) , e_bit);
                        break;
                    default:
                        break;
                }
            }
        }
    }

    bool Board::is_check(bool white_turn_)
    {
        compute_danger();
        if (white_turn_)
            return white_danger & king_wb;
        return black_danger & king_bb;
    }

    std::vector<Position> Board::get_white_king() {
        std::vector<Position> res = std::vector<Position>();
        unsigned long long int tmp = king_wb;
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            int floor_pow = utils::pow_two(floor);
            res.emplace_back(utils::to_position(floor_pow));
        } return res;
    }

    std::vector<Position> Board::get_white_queen() {
        std::vector<Position> res = std::vector<Position>();
        unsigned long long int tmp = queen_wb;
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            int floor_pow = utils::pow_two(floor);
            res.emplace_back(utils::to_position(floor_pow));
        } return res;
    }


    std::vector<Position> Board::get_white_knight() {
        std::vector<Position> res = std::vector<Position>();
        unsigned long long int tmp = knight_wb;
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            int floor_pow = utils::pow_two(floor);
            res.emplace_back(utils::to_position(floor_pow));
        } return res;
    }


    std::vector<Position> Board::get_white_bishop() {
        std::vector<Position> res = std::vector<Position>();
        unsigned long long int tmp = bishop_wb;
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            int floor_pow = utils::pow_two(floor);
            res.emplace_back(utils::to_position(floor_pow));
        } return res;
    }


    std::vector<Position> Board::get_white_rook() {
        std::vector<Position> res = std::vector<Position>();
        unsigned long long int tmp = rook_wb;
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            int floor_pow = utils::pow_two(floor);
            res.emplace_back(utils::to_position(floor_pow));
        } return res;
    }


    std::vector<Position> Board::get_white_pawn() {
        std::vector<Position> res = std::vector<Position>();
        unsigned long long int tmp = pawn_wb;
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            int floor_pow = utils::pow_two(floor);
            res.emplace_back(utils::to_position(floor_pow));
        } return res;
    }


    std::vector<Position> Board::get_black_king() {
        std::vector<Position> res = std::vector<Position>();
        unsigned long long int tmp = king_bb;
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            int floor_pow = utils::pow_two(floor);
            res.emplace_back(utils::to_position(floor_pow));
        } return res;
    }

    std::vector<Position> Board::get_black_queen() {
        std::vector<Position> res = std::vector<Position>();
        unsigned long long int tmp = queen_bb;
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            int floor_pow = utils::pow_two(floor);
            res.emplace_back(utils::to_position(floor_pow));
        } return res;
    }


    std::vector<Position> Board::get_black_knight() {
        std::vector<Position> res = std::vector<Position>();
        unsigned long long int tmp = knight_bb;
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            int floor_pow = utils::pow_two(floor);
            res.emplace_back(utils::to_position(floor_pow));
        } return res;
    }


    std::vector<Position> Board::get_black_bishop() {
        std::vector<Position> res = std::vector<Position>();
        unsigned long long int tmp = bishop_bb;
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            int floor_pow = utils::pow_two(floor);
            res.emplace_back(utils::to_position(floor_pow));
        } return res;
    }


    std::vector<Position> Board::get_black_rook() {
        std::vector<Position> res = std::vector<Position>();
        unsigned long long int tmp = rook_bb;
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            int floor_pow = utils::pow_two(floor);
            res.emplace_back(utils::to_position(floor_pow));
        } return res;
    }


    std::vector<Position> Board::get_black_pawn() {
        std::vector<Position> res = std::vector<Position>();
        unsigned long long int tmp = pawn_bb;
        while (tmp) {
            unsigned long long int floor = utils::floor_two(tmp);
            tmp -= floor;
            int floor_pow = utils::pow_two(floor);
            res.emplace_back(utils::to_position(floor_pow));
        } return res;
    }

    void Board::print()
    {
        for (int i = 7; i >= 0; i--)
        {
            std::cout << i + 1 << "|";
            for (int j = 7; j >= 0; j--)
            {
                long long int temp = utils::two_pow(i * 8 + j);
                if (king_wb & temp)
                    std::cout << "K";
                else if (queen_wb & temp)
                    std::cout << "Q";
                else if (knight_wb & temp)
                    std::cout << "N";
                else if (bishop_wb & temp)
                    std::cout << "B";
                else if (rook_wb & temp)
                    std::cout << "R";
                else if (pawn_wb & temp)
                    std::cout << "P";
                else if (king_bb & temp)
                    std::cout << "k";
                else if (queen_bb & temp)
                    std::cout << "q";
                else if (knight_bb & temp)
                    std::cout << "n";
                else if (bishop_bb & temp)
                    std::cout << "b";
                else if (rook_bb & temp)
                    std::cout << "r";
                else if (pawn_bb & temp)
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
