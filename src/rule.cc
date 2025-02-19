#include "rule.hh"
#define BLACK_QUEEN_CAST 8070450532247928832
#define WHITE_QUEEN_CAST 112

namespace board
{
    bool in_board_coord(File posX, Rank posY, int x, int y) {
        return static_cast<int>(posX) + x >= static_cast<int>(File::A) and
                static_cast<int>(posX) + x <= static_cast<int>(File::H) and
                static_cast<int>(posY) + y >= static_cast<int>(Rank::ONE) and
                static_cast<int>(posY) + y <= static_cast<int>(Rank::EIGHT);
    }

    bool in_board(Position& pos, int x, int y) {
        return static_cast<int>(pos.file_get()) + x
            >= static_cast<int>(File::A)
            and static_cast<int>(pos.file_get()) + x
            <= static_cast<int>(File::H)
            and static_cast<int>(pos.rank_get()) + y
            >= static_cast<int>(Rank::ONE)
            and static_cast<int>(pos.rank_get()) + y
            <= static_cast<int>(Rank::EIGHT);
    }

    void add_diag(Position& pos, std::vector<Move>& moves,
                                    Board& board, bool white_turn,
                                    std::pair<int, int>& direction,
                                    PieceType piece) {
        bit& ally = (white_turn) ? board.white_occupied_board
                            : board.black_occupied_board;
        int abs_factor = direction.first;
        int ord_factor = direction.second;
        File posX = pos.file_get();
        Rank posY = pos.rank_get();
        int x = static_cast<int>(posX);
        int y = static_cast<int>(posY);
        Position newPos(posX, posY);
        for (int i = 1;
                in_board_coord(posX, posY, abs_factor * i, ord_factor * i) and
                not board.is_occupied(ally,
                (newPos = Position(static_cast<File>(x + i * abs_factor),
                static_cast<Rank>(y + i * ord_factor)))); i++) {

            Move mv = Move(pos, newPos);
            opt_piecetype_t opt = board.is_occupied(newPos,
                        (white_turn) ? Color::BLACK : Color::WHITE);
            if (opt.has_value()) {
                mv.capture_set(opt.value());
                mv.piece_set(piece);
                moves.push_back(mv);
                break;
            } else {
                mv.piece_set(piece);
                moves.push_back(mv);
            }
        }
    }

    void set_direction(int& abs, int& ord, int direction) {
        switch (direction) {
        case 7:
            abs = 1;
            ord = 1;
            break;
        case 9:
            abs = -1;
            ord = 1;
            break;
        case -7:
            abs = 1;
            ord = -1;
            break;
        case -9:
            abs = -1;
            ord = -1;
            break;
        case -1:
            abs = 1;
            ord = 0;
            break;
        case 1:
            abs = -1;
            ord = 0;
            break;
        case 8:
            abs = 0;
            ord = 1;
            break;
        case -8:
            abs = 0;
            ord = -1;
            break;
        default:
            break;
        }
    }

/*
**   9  8  7
**    \ | /
** 1 <- p -> -1
**    / | \
**  -7 -8  -9
*/

    inline bool is_double_top(int direction) {
        return direction == 15 or direction == 17;
    }

    inline bool is_double_bot(int direction) {
        return direction == -15 or direction == -17;
     }

    inline bool is_double_right(int direction) {
        return direction == 6 or direction == -10;
    }

    inline bool is_double_left(int direction) {
        return direction == -6 or direction == 10;
    }

    inline bool is_top(int direction) {
        return direction == 9 or direction == 8 or direction == 7
                or direction == 6  or direction == 10
                or is_double_top(direction);
    }

    inline bool is_bot(int direction) {
        return direction == -9 or direction == -8 or direction == -7
                or direction == -6 or direction == -10
                or is_double_bot(direction);
     }

    inline bool is_right(int direction) {
        return direction == 7 or direction == -1 or direction == -9
                or direction == 15 or direction == -17
                or is_double_right(direction);
    }

    inline bool is_left(int direction) {
        return direction == 9 or direction == 1 or direction == -7
                or direction == -15 or direction == 17
                or is_double_left(direction);
    }

    bool is_max_pos(Position& pos, int direction) {
        return (pos.rank_get() == Rank::EIGHT and is_top(direction))
                or (pos.rank_get() == Rank::SEVEN and is_double_top(direction))
                or (pos.rank_get() == Rank::ONE and is_bot(direction))
                or (pos.rank_get() == Rank::TWO and is_double_bot(direction))
                or (pos.file_get() == File::H and is_right(direction))
                or (pos.file_get() == File::G and is_double_right(direction))
                or (pos.file_get() == File::B and is_double_left(direction))
                or (pos.file_get() == File::A and is_left(direction));
    }

    void add_moves(unsigned long long int posbit, std::vector<Move>& moves,
                                    Board& board, bool white_turn,
                                    int direction,
                                    PieceType piece) {
        unsigned long long int ally =
                            ((white_turn) ? board.white_occupied_board
                            : board.black_occupied_board);


        Position pos(utils::get_position(posbit));
        Position newPos(File::A, Rank::ONE);

        bool overload = false;
        newPos = utils::get_position(posbit);
        while (not (overload = is_max_pos(newPos, direction))
            and not (((direction < 0) ? posbit = posbit >> -direction
                                      : posbit = posbit << direction) & ally))
        {
            newPos = utils::get_position(posbit);
            Move mv = Move(pos, newPos);
            opt_piecetype_t opt = board.is_occupied(newPos,
                        (white_turn) ? Color::BLACK : Color::WHITE);
            if (opt.has_value()) {
                mv.capture_set(opt.value());
                mv.piece_set(piece);
                moves.push_back(mv);
                break;
            } else {
                mv.piece_set(piece);
                moves.push_back(mv);
            }
        }
    }

    void add_diags(unsigned long long int pos, std::vector<Move>& moves,
                                    Board& board, bool white_turn,
                                    PieceType piece) {

        add_moves(pos, moves, board, white_turn, 9, piece);
        add_moves(pos, moves, board, white_turn, 7, piece);
        add_moves(pos, moves, board, white_turn, -7, piece);
        add_moves(pos, moves, board, white_turn, -9, piece);
    }

    void add_xys(unsigned long long int pos, std::vector<Move>& moves,
                                    Board& board, bool white_turn,
                                    PieceType piece) {
        add_moves(pos, moves, board, white_turn, 8, piece);
        add_moves(pos, moves, board, white_turn, -8, piece);
        add_moves(pos, moves, board, white_turn, 1, piece);
        add_moves(pos, moves, board, white_turn, -1, piece);
    }

    bool in_board(int x, int y)
    {
        return x >= static_cast<int>(File::A) and
               x <= static_cast<int>(File::H) and
               y >= static_cast<int>(Rank::ONE) and
               y <= static_cast<int>(Rank::EIGHT);
    }


    void add_forward(Position& pos, std::vector<Move>& moves,
                        Board& board, bool white_turn, PieceType piece)
    {
        auto ally = (white_turn) ? board.white_occupied_board
                            : board.black_occupied_board;
        File file = pos.file_get();
        int f = static_cast<int>(pos.file_get());
        int r = static_cast<int>(pos.rank_get());
        for (int i = 1; i < 8 - r; i++)
        {
            Position new_pos = Position(file, static_cast<Rank>(r + i));
            if (in_board(f, r + i))
            {
                opt_piecetype_t opt = board.is_occupied(new_pos,
                            (white_turn) ? Color::BLACK : Color::WHITE);
                Move mv = Move(pos, new_pos);
                if (board.is_occupied(ally, new_pos))
                {
                    return;
                }
                else if (opt.has_value())
                {
                    mv.piece_set(piece);
                    mv.capture_set(opt.value());
                    moves.push_back(mv);
                }
                else
                {
                    mv.piece_set(piece);
                    moves.push_back(mv);
                }
            }
            else
            {
                return;
            }
        }
    }


    void add_promotion(std::vector<Move>& moves, Position& pos,
                Position& newpos, int capture) {

        for (int i = 0; i < 4; i++) {
            Move mv(pos, newpos);
            mv.piece_set(PieceType::PAWN);
            mv.promotion_set(piecetype_array[i]);
            if (capture >= 0)
                mv.capture_set(static_cast<PieceType>(capture));
            moves.push_back(mv);
        }
    }

      void add_move_bis(unsigned long long int posbit,
                                    std::vector<Move>& moves,
                                    Board& board, bool white_turn,
                                    int direction,
                                    PieceType piece)
    {
        unsigned long long int ally =
                            ((white_turn) ? board.white_occupied_board
                            : board.black_occupied_board);

        Position pos(utils::get_position(posbit));
        Position newPos(File::A, Rank::ONE);
        bool overload = false;
        newPos = utils::get_position(posbit);
        if (not (overload = is_max_pos(newPos, direction))
            and not (((direction < 0) ? posbit = posbit >> -direction
                                      : posbit = posbit << direction) & ally))
        {
            newPos = utils::get_position(posbit);
            Move mv = Move(pos, newPos);
            opt_piecetype_t opt = board.is_occupied(newPos,
                        (white_turn) ? Color::BLACK : Color::WHITE);
            if (opt.has_value())
            {
                if (piece == PieceType::PAWN and ((white_turn
                    and pos.rank_get() == Rank::SEVEN) or
                    (not white_turn and pos.rank_get() == Rank::TWO)))
                {
                    add_promotion(moves, pos, newPos,
                                    static_cast<int>(opt.value()));
                }
                else
                {
                    Move mv = Move(pos, newPos);
                    mv.capture_set(opt.value());
                    mv.piece_set(piece);
                    moves.push_back(mv);
                }
            }
            else if (piece != PieceType::PAWN)
            {
                mv.piece_set(piece);
                moves.push_back(mv);
            }
        }
    }

    void single_step(unsigned long long int pawn, Board& board,
                        bool white_turn, std::vector<Move>& moves)
    {
        int color = (white_turn) ? 0 : 1;
        unsigned long long int new_pos = (pawn << 8) >> (color << 4);
        unsigned long long int occupied = board.occupied_board;
        if (not (new_pos & occupied))
        {
            Position begin = utils::get_position(pawn);
            Position end = utils::get_position(new_pos);
            if ((white_turn and begin.rank_get() == Rank::SEVEN) or
                (not white_turn and begin.rank_get() == Rank::TWO))
            {
                add_promotion(moves, begin, end, -1);
            }
            else
            {
                Move mv = Move(begin, end);
                mv.piece_set(PieceType::PAWN);
                moves.push_back(mv);
            }
        }
        if (white_turn)
        {
            add_move_bis(pawn, moves, board, white_turn, 7, PieceType::PAWN);
            add_move_bis(pawn, moves, board, white_turn, 9, PieceType::PAWN);
        }
        else
        {
            add_move_bis(pawn, moves, board, white_turn, -7, PieceType::PAWN);
            add_move_bis(pawn, moves, board, white_turn, -9, PieceType::PAWN);
        }
   }


    void double_step(unsigned long long int pawn, Board& board,
                        bool white_turn, std::vector<Move>& moves)
    {
        int color = (white_turn) ? 0 : 1;
        unsigned long long int new_pos = (white_turn) ? (pawn << 16)
                                                    : (pawn >> 16);
        unsigned long long int new_pos2 = (pawn << 8) >> (color << 4);
        unsigned long long int occupied = board.occupied_board;
        if (not (new_pos & occupied) and not (new_pos2 & occupied))
        {
            Position begin = utils::get_position(pawn);
            if ((white_turn and begin.rank_get() == Rank::TWO) or
                (not white_turn and begin.rank_get() == Rank::SEVEN))
            {
                Position end = utils::get_position(new_pos);
                Move mv = Move(begin, end);
                mv.piece_set(PieceType::PAWN);
                moves.push_back(mv);
            }
        }
    }



    void Rule::generate_pawn_moves(Chessboard& chessboard,
                                    std::vector<Move>& moves)
    {
        Board board = chessboard.getBoard();
        bool white_turn = chessboard.isWhiteTurn();
        unsigned long long int pawns = (white_turn) ? board.pawn_wb
                                                    : board.pawn_bb;
        unsigned long long int ally = (white_turn)
                                        ? board.white_occupied_board
                                        : board.black_occupied_board;

        unsigned long long int acc = 0;
        unsigned long long int pawn = 0;
        while (acc < pawns)
        {
            pawn = pawns ^ (pawns & (pawns - acc - 1));
            single_step(pawn, board, white_turn, moves);
            double_step(pawn, board, white_turn, moves);
            auto  en_passant = chessboard.getEnPassantBitboard();
            if (en_passant and not (en_passant & ally))
            {
                Position begin = utils::get_position(pawn);
                unsigned long long int left_pos = (white_turn) ? pawn << 9
                                                    : pawn >> 7;
                unsigned long long int right_pos = (white_turn) ? pawn << 7
                                                    : pawn >> 9;
                if (left_pos == en_passant or right_pos == en_passant)
                {
                    Position end = utils::get_position(en_passant);
                    Move mv = Move(begin, end);
                    mv.piece_set(PieceType::PAWN);
                    mv.en_passant_set(true);
                    mv.capture_set(PieceType::PAWN);
                    moves.push_back(mv);
                }
            }
            acc |= pawn;
        }
    }

    bool occupied(Board& board, int x, int y, bit&& bitboard) {
        Position pos(static_cast<File>(x), static_cast<Rank>(y));
        return in_board(x, y) and board.is_occupied(bitboard, pos);
    }


    void check_king_castling(std::vector<Move>& moves,
                        unsigned long long int pos, bool white_turn,
                        Board& board)
    {
        unsigned long long int occupied = board.occupied_board;
        unsigned long long int tmp = (white_turn) ? 6
                                        : pow(2, 58) + pow(2, 57);
        if (pos == ((white_turn) ? 8 : pow(2, 59)) and not(tmp & occupied))
        {
            Position begin = utils::get_position(pos);
            Position end = utils::get_position(pos >> 2);
            Move mv = Move(begin, end);
            mv.king_castling_set(true);
            mv.piece_set(PieceType::KING);
            moves.push_back(mv);
        }
    }

    void check_queen_castling(std::vector<Move>& moves,
                        unsigned long long int pos, bool white_turn,
                        Board& board)
    {
        unsigned long long int occupied = board.occupied_board;
        unsigned long long int block = (white_turn) ? WHITE_QUEEN_CAST
                                    : BLACK_QUEEN_CAST;
        if (pos == ((white_turn) ? 8 : pow(2, 59)) and not (block & occupied))
        {
            Position begin = utils::get_position(pos);
            Position end = utils::get_position(pos << 2);
            Move mv = Move(begin, end);
            mv.queen_castling_set(true);
            mv.piece_set(PieceType::KING);
            moves.push_back(mv);
        }
    }




    void add_single_xys(unsigned long long int pos, std::vector<Move>& moves,
                                    Board& board, bool white_turn,
                                    PieceType piece)
    {
        add_move_bis(pos, moves, board, white_turn, 8, piece);
        add_move_bis(pos, moves, board, white_turn, -8, piece);
        add_move_bis(pos, moves, board, white_turn, 1, piece);
        add_move_bis(pos, moves, board, white_turn, -1, piece);
        add_move_bis(pos, moves, board, white_turn, -7, piece);
        add_move_bis(pos, moves, board, white_turn, 7, piece);
        add_move_bis(pos, moves, board, white_turn, 9, piece);
        add_move_bis(pos, moves, board, white_turn, -9, piece);
    }


    void Rule::generate_king_moves(Chessboard& chessboard,
                                    std::vector<Move>& moves)
    {
        Board board = chessboard.getBoard();
        bool white_turn = chessboard.isWhiteTurn();
        unsigned long long int kings = (white_turn) ? board.king_wb
                                                    : board.king_bb;


        unsigned long long int king = kings ^ (kings & (kings - 1));
        bool king_castling = (white_turn) ? chessboard.getWhiteKingCastling()
                                    : chessboard.getBlackKingCastling();
        bool queen_castling = (white_turn) ? chessboard.getWhiteQueenCastling()
                                    : chessboard.getBlackQueenCastling();

        if (!chessboard.is_check(white_turn)) {
            if (king_castling)
            {
                check_king_castling(moves, king, white_turn, board);
            }
            if (queen_castling)
            {
                check_queen_castling(moves, king, white_turn, board);
            }
        }

        add_single_xys(king, moves, board, white_turn, PieceType::KING);
    }

    void Rule::generate_rook_moves(Chessboard& chessboard,
                                    std::vector<Move>& moves)
    {
        Board board = chessboard.getBoard();
        bool white_turn = chessboard.isWhiteTurn();
        unsigned long long int rook = ((white_turn)
                            ? board.rook_wb : board.rook_bb);
        unsigned long long int acc = 0;
        unsigned long long int pos = 0;
       while (acc < rook) {
            pos = rook ^ (rook & (rook - acc - 1));
            add_xys(pos, moves, board, white_turn, PieceType::ROOK);
            acc |= pos;
        }
    }

    void Rule::generate_queen_moves(Chessboard& chessboard,
                                    std::vector<Move>& moves)
    {
        Board board = chessboard.getBoard();
        bool white_turn = chessboard.isWhiteTurn();
        unsigned long long int queen = ((white_turn)
                            ? board.queen_wb : board.queen_bb);
        unsigned long long int acc = 0;
        unsigned long long int pos = 0;

        while (acc < queen) {
            pos = queen ^ (queen & (queen - acc - 1));

            add_diags(pos, moves, board, white_turn, PieceType::QUEEN);
            add_xys(pos, moves, board, white_turn, PieceType::QUEEN);
            acc |= pos;
        }
    }

    void Rule::generate_bishop_moves(Chessboard& chessboard,
                                    std::vector<Move>& moves) {
        Board board = chessboard.getBoard();
        bool white_turn = chessboard.isWhiteTurn();
        unsigned long long int bishop = ((white_turn)
                            ? board.bishop_wb : board.bishop_bb);
        unsigned long long int acc = 0;
        unsigned long long int pos = 0;

        while (acc < bishop) {
            pos = bishop ^ (bishop & (bishop - acc - 1));
            add_diags(pos, moves, board, white_turn, PieceType::BISHOP);
            acc |= pos;
        }
    }

    void Rule::generate_knight_moves(Chessboard& chessboard,
                                    std::vector<Move>& moves)
    {
        Board board = chessboard.getBoard();
        bool white_turn = chessboard.isWhiteTurn();
        unsigned long long int knight = ((white_turn)
                            ? board.knight_wb : board.knight_bb);
        unsigned long long int acc = 0;
        unsigned long long int pos = 0;

        while (acc < knight) {
            pos = knight ^ (knight & (knight - acc - 1));
            add_move_bis(pos, moves, board, white_turn, 17, PieceType::KNIGHT);
            add_move_bis(pos, moves, board, white_turn, 15, PieceType::KNIGHT);
            add_move_bis(pos, moves, board, white_turn, 10, PieceType::KNIGHT);
            add_move_bis(pos, moves, board, white_turn, 6, PieceType::KNIGHT);
            add_move_bis(pos, moves, board, white_turn, -6, PieceType::KNIGHT);
            add_move_bis(pos, moves, board, white_turn, -10,
                                                    PieceType::KNIGHT);
            add_move_bis(pos, moves, board, white_turn, -15,
                                                    PieceType::KNIGHT);
            add_move_bis(pos, moves, board, white_turn, -17,
                                                    PieceType::KNIGHT);
            acc |= pos;
        }
    }
}
