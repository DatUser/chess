#include "rule.hh"

namespace board
{
    bool in_board_coord(File posX, Rank posY, int x, int y) {
        return static_cast<int>(posX) + x >= static_cast<int>(File::A) and
                static_cast<int>(posX) + x <= static_cast<int>(File::H) and
                static_cast<int>(posY) + y >= static_cast<int>(Rank::ONE) and
                static_cast<int>(posY) + y <= static_cast<int>(Rank::EIGHT);
    }

    bool in_board(Position pos, int x, int y) {
        return static_cast<int>(pos.file_get()) + x
            >= static_cast<int>(File::A)
            and static_cast<int>(pos.file_get()) + x
            <= static_cast<int>(File::H)
            and static_cast<int>(pos.rank_get()) + y
            >= static_cast<int>(Rank::ONE)
            and static_cast<int>(pos.rank_get()) + y
            <= static_cast<int>(Rank::EIGHT);
    }

    void add_diag(Position pos, std::vector<Move>& moves,
                                    Board& board, bool white_turn,
                                    std::pair<int, int>& direction,
                                    PieceType piece) {
        shared_bit ally = (white_turn) ? board.white_occupied_board_get()
                            : board.black_occupied_board_get();
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

    inline bool is_top(int direction) {
        return direction == 9 or direction == 8 or direction == 7;
    }

    inline bool is_bot(int direction) {
        return direction == -9 or direction == -8 or direction == -7;
    }

    inline bool is_right(int direction) {
        return direction == 7 or direction == -1 or direction == -9;
    }

    inline bool is_left(int direction) {
        return direction == 9 or direction == 1 or direction == -7;
    }

    bool is_max_pos(Position& pos, int direction) {
        return (pos.rank_get() == Rank::EIGHT and is_top(direction))
                or (pos.rank_get() == Rank::ONE and is_bot(direction))
                or (pos.file_get() == File::H and is_right(direction))
                or (pos.file_get() == File::A and is_left(direction));
    }

    void add_moves(unsigned long long int posbit, std::vector<Move>& moves,
                                    Board& board, bool white_turn,
                                    int direction,
                                    //std::pair<int, int>& direction,
                                    PieceType piece) {
        unsigned long long int ally =
                            ((white_turn) ? board.white_occupied_board_get()
                            : board.black_occupied_board_get())->board_get();

        //int abs_factor = 0;
        //int ord_factor = 0;
        //set_direction(abs_factor, ord_factor, direction);
        Position pos(utils::get_position(posbit));
        Position newPos(File::A, Rank::ONE);
        //File posX = pos.file_get();
        //Rank posY = pos.rank_get();
        //int x = static_cast<int>(posX);
        //int y = static_cast<int>(posY);
        bool overload = false;
        newPos = utils::get_position(posbit);
        while (not (overload = is_max_pos(newPos, direction))
            and not (((direction < 0) ? posbit = posbit >> -direction
                                      : posbit = posbit << direction) & ally)) {
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

    void add_diags(/*Position*/unsigned long long int pos, std::vector<Move>& moves,
                                    Board& board, bool white_turn,
                                    PieceType piece) {
        /*std::pair<int, int> direction(-1, -1);
        add_diag(pos, moves, board, white_turn, direction, piece);
        add_diag(pos, moves, board, white_turn,
                            (direction = std::pair<int, int>(1, -1)), piece);
        add_diag(pos, moves, board, white_turn,
                            (direction = std::pair<int, int>(-1, 1)), piece);
        add_diag(pos, moves, board, white_turn,
                            (direction = std::pair<int, int>(1, 1)), piece);*/
        add_moves(pos, moves, board, white_turn, 9, piece);
        add_moves(pos, moves, board, white_turn, 7, piece);
        add_moves(pos, moves, board, white_turn, -7, piece);
        add_moves(pos, moves, board, white_turn, -9, piece);
    }

    void add_xys(/*Position*/unsigned long long int pos, std::vector<Move>& moves,
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


    void add_forward(Position pos, std::vector<Move>& moves,
                        Board board, bool white_turn, PieceType piece)
    {
        auto ally = (white_turn) ? board.white_occupied_board_get()
                            : board.black_occupied_board_get();
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

    /*void add_backward(Position pos, std::vector<Move>& moves,
                        Board board, bool white_turn, PieceType piece)
    {
        auto ally = (white_turn) ? board.white_occupied_board_get()
                            : board.black_occupied_board_get();
        File file = pos.file_get();
        int f = static_cast<int>(pos.file_get());
        int r = static_cast<int>(pos.rank_get());
        for (int i = 1; i < 8 - r; i++)
        {
            Position new_pos = Position(file, static_cast<Rank>(r - i));
            if (in_board(f, r - i))
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

    void add_rightward(Position pos, std::vector<Move>& moves,
                            Board board, bool white_turn, PieceType piece)
    {
        auto ally = (white_turn) ? board.white_occupied_board_get()
                            : board.black_occupied_board_get();
        Rank rank = pos.rank_get();
        int f = static_cast<int>(pos.file_get());
        int r = static_cast<int>(pos.rank_get());
        for (int i = 1; i < 8; i++)
        {
            Position new_pos = Position(static_cast<File>(f + i), rank);
            if (in_board(f + i, r))
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

    void add_leftward(Position pos, std::vector<Move>& moves,
                        Board board, bool white_turn, PieceType piece)
    {
        auto ally = (white_turn) ? board.white_occupied_board_get()
                            : board.black_occupied_board_get();
        Rank rank = pos.rank_get();
        int f = static_cast<int>(pos.file_get());
        int r = static_cast<int>(pos.rank_get());
        for (int i = 1; i < 8; i++)
        {
            Position new_pos = Position(static_cast<File>(f - i), rank);
            if (in_board(f - i, r))
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
    }*/

    void pawn_step(std::vector<Move>& moves, Position pos, Board board,
            bool white_turn) {
        int y = static_cast<int>(pos.rank_get()) + 1 * ((white_turn) ? 1 : -1);
        Position newpos(pos.file_get(), static_cast<Rank>(y));

        if (in_board(newpos, 0, 0) and
            not board.is_occupied(board.occupied_board_get(), newpos)) {
            Move mv(pos, newpos);
            mv.piece_set(PieceType::PAWN);
            moves.push_back(mv);
        }
    }

    void add_promotion(std::vector<Move>& moves, Position pos, Position newpos,
                int capture) {

        for (int i = 0; i < 4; i++) {
            Move mv(pos, newpos);
            mv.piece_set(PieceType::PAWN);
            mv.promotion_set(piecetype_array[i]);
            if (capture >= 0)
                mv.capture_set(static_cast<PieceType>(capture));
            moves.push_back(mv);
        }
    }

    //Since it is at the sames place we deal with it in the same funcs
    void pawn_double_step_promotion(std::vector<Move>& moves, Position pos,
            Board board, bool doubl, bool white_turn) {
        int y = static_cast<int>(pos.rank_get()) +
                ((doubl) ? 2  * ((white_turn) ? 1 : -1)
                : 1 * ((white_turn) ? 1 : -1));
        Position newpos(pos.file_get(), static_cast<Rank>(y));
        if (in_board(newpos, 0, 0) and
            not board.is_occupied(board.occupied_board_get(), newpos)) {
            Move mv(pos, newpos);
            mv.piece_set(PieceType::PAWN);
            //GERER LE EN PASSANT
            if (doubl) {
                mv.double_pawn_push_set(true);
                moves.push_back(mv);
            }
             else
                add_promotion(moves, pos, newpos, -1);
        }
    }

    bool pos_equal(Position posA, Position posB) {
        return posA.rank_get() == posB.rank_get() and
                posA.file_get() == posB.file_get();
    }

    /*void pawn_eat(std::vector<Move>& moves, Position pos,
            Chessboard chessboard, bool white_turn) {
        int x_r = static_cast<int>(pos.file_get()) + 1;
        int x_l = static_cast<int>(pos.file_get()) - 1;
        int y = static_cast<int>(pos.rank_get()) + 1 * ((white_turn) ? 1 : -1);

        Position right(static_cast<File>(x_r), static_cast<Rank>(y));
        Position left(static_cast<File>(x_l), static_cast<Rank>(y));

        Position en_passant_right(static_cast<File>(x_r), pos.rank_get());
        Position en_passant_left(static_cast<File>(x_l), pos.rank_get());

        Board board = chessboard.getBoard();

        auto en_passant_ = chessboard.getEnPassant();
        opt_piecetype_t opt_r = board.is_occupied(right,
                            (white_turn) ? Color::BLACK : Color::WHITE);
        opt_piecetype_t opt_l = board.is_occupied(left,
                            (white_turn) ? Color::BLACK : Color::WHITE);

        if (in_board(pos, -1, 1 * ((white_turn) ? 1 : -1))
                and opt_l.has_value()) {
            if (left.rank_get() != Rank::EIGHT
                and left.rank_get() != Rank::ONE) {
                Move mv(pos, left);
                mv.piece_set(PieceType::PAWN);
                mv.capture_set(opt_l.value());
                moves.push_back(mv);
            } else {
                add_promotion(moves, pos, left,
                            static_cast<int>(opt_l.value()));
            }
        }

        if (in_board(pos, 1, 1 * ((white_turn) ? 1 : -1))
                        and opt_r.has_value()) {
            if (right.rank_get() != Rank::EIGHT
                and right.rank_get() != Rank::ONE) {
                Move mv(pos, right);
                mv.piece_set(PieceType::PAWN);
                mv.capture_set(opt_r.value());
                moves.push_back(mv);
            } else {
                add_promotion(moves, pos, right,
                        static_cast<int>(opt_r.value()));
            }
        }

        if (en_passant_.has_value()) {
            if (in_board(pos, -1, 0)
                and pos_equal(en_passant_.value(), en_passant_left)) {
                Move mv(pos, left);
                mv.piece_set(PieceType::PAWN);
                //mv.capture_set(en_passant_.value());
                mv.en_passant_set(true);
                moves.push_back(mv);
            }

            else if (in_board(pos, 1, 0)
                and pos_equal(en_passant_.value(), en_passant_right)) {
                Move mv(pos, right);
                mv.piece_set(PieceType::PAWN);
                //mv.capture_set(en_passant_.value());
                mv.en_passant_set(true);
                moves.push_back(mv);

            }
        }
    }*/

    /*std::vector<Move> Rule::generate_pawn_moves(Chessboard chessboard)
    {
        std::vector<Move> m;
        Board board = chessboard.getBoard();
        bool white_turn = chessboard.isWhiteTurn();
        std::vector<Position> pawns = (white_turn) ? board.get_white_pawn()
                            : board.get_black_pawn();
        for (unsigned int i = 0; i < pawns.size(); i++) {
            switch (pawns[i].rank_get()) {
            case Rank::ONE:
            case Rank::EIGHT:
                break;

            case Rank::TWO:
                pawn_step(m, pawns[i], board, white_turn);
                pawn_double_step_promotion(m, pawns[i], board,
                                            white_turn, white_turn);
                pawn_eat(m, pawns[i], chessboard, white_turn);
                break;

            case Rank::SEVEN:
                pawn_step(m, pawns[i], board, white_turn);
                pawn_double_step_promotion(m, pawns[i], board,
                                            not white_turn, white_turn);
                pawn_eat(m, pawns[i], chessboard, white_turn);
                break;

            default:
                pawn_step(m, pawns[i], board, white_turn);
                pawn_eat(m, pawns[i], chessboard, white_turn);
                break;
            }
        }

        return m;
    }*/

    /*void pawn_step(std::vector<Move>& moves, Position pos, Board board,
            bool white_turn) {
        int y = static_cast<int>(pos.rank_get()) + 1 * ((white_turn) ? 1 : -1);
        Position newpos(pos.file_get(), static_cast<Rank>(y));

        if (in_board(newpos, 0, 0) and
            not board.is_occupied(board.occupied_board_get(), newpos)) {
            Move mv(pos, newpos);
            mv.piece_set(PieceType::PAWN);
            moves.push_back(mv);
        }
    }*/


    void add_move_bis(unsigned long long int posbit, std::vector<Move>& moves,
                                    Board& board, bool white_turn,
                                    int direction,
                                    //std::pair<int, int>& direction,
                                    PieceType piece)
    {
        unsigned long long int ally =
                            ((white_turn) ? board.white_occupied_board_get()
                            : board.black_occupied_board_get())->board_get();

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
                if (piece == PieceType::PAWN and ((white_turn and pos.rank_get() == Rank::SEVEN) or 
                    (not white_turn and pos.rank_get() == Rank::TWO)))
                {
                    add_promotion(moves, pos, newPos, static_cast<int>(opt.value()));
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

    void single_step(unsigned long long int pawn, Board board, bool white_turn,
                     std::vector<Move>& moves)
    {
        int color = (white_turn) ? 0 : 1;
        unsigned long long int new_pos = (pawn << 8) >> (color << 4);
        unsigned long long int occupied = board.occupied_board.get()->board_get();
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


    void double_step(unsigned long long int pawn, Board board, bool white_turn,
                     std::vector<Move>& moves)
    {
        int color = (white_turn) ? 0 : 1;
        unsigned long long int new_pos = (pawn << 16) >> (color << 5);
        unsigned long long int occupied = board.occupied_board.get()->board_get();
        if (not (new_pos & occupied))
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





    std::vector<Move> Rule::generate_pawn_moves(Chessboard chessboard)
    {
        std::vector<Move> moves;
        Board board = chessboard.getBoard();
        bool white_turn = chessboard.isWhiteTurn();
        unsigned long long int pawns = (white_turn) ? board.pawn_wb.get()->board_get() 
                                                    : board.pawn_bb.get()->board_get();


        unsigned long long int acc = 0;
        unsigned long long int pawn = 0;
        while (acc < pawns)
        {
            pawn = pawns ^ (pawns & (pawns - acc - 1));
            single_step(pawn, board, white_turn, moves);
            double_step(pawn, board, white_turn, moves);
            if (chessboard.getEnPassant().has_value())
            {
                //Position begin = utils::get_position(pawn);
            }
            acc |= pawn;
        }

        return moves;
    }

    bool occupied(Board& board, int x, int y, shared_bit& bitboard) {
        Position pos(static_cast<File>(x), static_cast<Rank>(y));
        return in_board(x, y) and board.is_occupied(bitboard, pos);
    }

    void check_king_castling(std::vector<Move>& moves,
                        Position pos, bool white_turn, Board board)
    {
        //auto board = chessboard.getBoard();
        auto occupied = board.occupied_board_get();
        int f = static_cast<int>(pos.file_get());
        int r = static_cast<int>(pos.rank_get());
        Rank rank = pos.rank_get();

        if (in_board(f + 3, r)) {
            Position pos1 = Position(static_cast<File>(f + 1), rank);
            Position pos2 = Position(static_cast<File>(f + 2), rank);
            Position posTower = Position(static_cast<File>(f + 3), rank);
            opt_piecetype_t opt = board.is_occupied(posTower,
                            (white_turn) ? Color::WHITE : Color::BLACK);

            if (in_board(f + 1, r) and not board.is_occupied(occupied, pos1)
                and in_board(f + 2, r) and
                not board.is_occupied(occupied, pos2)
                and opt.has_value() and opt.value() == PieceType::ROOK) {
                    //indeed it is the pos just before the tower
                    Move mv = Move(pos, pos2);
                    mv.piece_set(PieceType::KING);
                    mv.king_castling_set(true);
                    moves.push_back(mv);
                }
        }
    }


    void check_queen_castling(std::vector<Move>& moves,
                        Position pos, bool white_turn, Board board)
    {

        auto occupied = board.occupied_board_get();
        int f = static_cast<int>(pos.file_get());
        int r = static_cast<int>(pos.rank_get());
        Rank rank = pos.rank_get();

        if (in_board(f - 3, r))
        {
            Position pos1 = Position(static_cast<File>(f - 1), rank);
            Position pos2 = Position(static_cast<File>(f - 2), rank);
            Position posTower = Position(static_cast<File>(f - 3), rank);
            opt_piecetype_t opt = board.is_occupied(posTower,
                            (white_turn) ? Color::WHITE : Color::BLACK);

            if (in_board(f - 1, r) and not board.is_occupied(occupied, pos1)
                and in_board(f - 2, r) and not board.is_occupied(occupied, pos2)
                and opt.has_value() and opt.value() == PieceType::ROOK)
                {
                    Move mv = Move(pos, pos2);
                    mv.king_castling_set(true);
                    mv.piece_set(PieceType::KING);
                    moves.push_back(mv);
                }
        }
    }

    std::vector<Move> Rule::generate_king_moves(Chessboard chessboard)
    {
        std::vector<Move> m;
        Board board = chessboard.getBoard();
        std::vector<Position> positions = (chessboard.isWhiteTurn()) ?
                                          board.get_white_king()
                                          : board.get_black_king();

        auto allies = (chessboard.isWhiteTurn()) ?
                           board.white_occupied_board_get()
                           : board.black_occupied_board_get();

        auto enemies = (chessboard.isWhiteTurn()) ?
                           board.black_occupied_board_get()
                           : board.white_occupied_board_get();

        if (chessboard.isWhiteTurn())
        {
            positions = board.get_white_king();
        }
        else
        {
            positions = board.get_black_king();
        }
        bool white_turn = chessboard.isWhiteTurn();
        bool king_castling = (white_turn) ? chessboard.getWhiteKingCastling()
                                    : chessboard.getBlackKingCastling();
        bool queen_castling = (white_turn) ? chessboard.getWhiteQueenCastling()
                                    : chessboard.getBlackQueenCastling();

        for (Position p : positions)
        {
            if (king_castling)
            {
                check_king_castling(m, p, chessboard.isWhiteTurn(), board);
            }
            if (queen_castling)
            {
                check_queen_castling(m, p, chessboard.isWhiteTurn(), board);
            }

            File file = p.file_get();
            Rank rank = p.rank_get();
            int f = static_cast<int>(p.file_get());
            int r = static_cast<int>(p.rank_get());

            Position pos1 = Position(static_cast<File>(f - 1),
                                        static_cast<Rank>(r + 1));
            Position pos2 = Position(file, static_cast<Rank>(r + 1));
            Position pos3 = Position(static_cast<File>(f + 1),
                                        static_cast<Rank>(r + 1));
            Position pos4 = Position(static_cast<File>(f - 1), rank);
            Position pos5 = Position(static_cast<File>(f + 1), rank);
            Position pos6 = Position(static_cast<File>(f - 1),
                                        static_cast<Rank>(r - 1));
            Position pos7 = Position(file, static_cast<Rank>(r - 1));
            Position pos8 = Position(static_cast<File>(f + 1),
                                        static_cast<Rank>(r - 1));

            PieceType piece = PieceType::KING;

            if (in_board(f - 1, r + 1) and not board.is_occupied(allies, pos1))
            {
                Move mv = Move(p, pos1);
                mv.piece_set(piece);
                if (board.is_occupied(enemies, pos1))
                {
                    m.push_back(mv);
                }
                else
                {
                    m.push_back(mv);
                }
            }
            if (in_board(f, r + 1) and not board.is_occupied(allies, pos2))
            {
                Move mv = Move(p, pos2);
                mv.piece_set(piece);
                if (board.is_occupied(enemies, pos2))
                {
                    m.push_back(mv);
                }
                else
                {
                    m.push_back(mv);
                }
            }
            if (in_board(f + 1, r + 1) and not board.is_occupied(allies, pos3))
            {
                Move mv = Move(p, pos3);
                mv.piece_set(piece);
                if (board.is_occupied(enemies, pos3))
                {
                    m.push_back(mv);
                }
                else
                {
                    m.push_back(mv);
                }
            }
            if (in_board(f - 1, r) and not board.is_occupied(allies, pos4))
            {
                Move mv = Move(p, pos4);
                mv.piece_set(piece);
                if (board.is_occupied(enemies, pos4))
                {
                    m.push_back(mv);
                }
                else
                {
                    m.push_back(mv);
                }
            }
            if (in_board(f + 1, r) and not board.is_occupied(allies, pos5))
            {
                Move mv = Move(p, pos5);
                mv.piece_set(piece);
                if (board.is_occupied(enemies, pos5))
                {
                    m.push_back(mv);
                }
                else
                {
                    m.push_back(mv);
                }
            }
            if (in_board(f - 1, r - 1) and not board.is_occupied(allies, pos6))
            {
                Move mv = Move(p, pos6);
                mv.piece_set(piece);
                if (board.is_occupied(enemies, pos6))
                {
                    m.push_back(mv);
                }
                else
                {
                    m.push_back(mv);
                }
            }
            if (in_board(f, r - 1) and not board.is_occupied(allies, pos7))
            {
                Move mv = Move(p, pos7);
                mv.piece_set(piece);
                if (board.is_occupied(enemies, pos7))
                {
                    m.push_back(mv);
                }
                else
                {
                    m.push_back(mv);
                }
            }
            if (in_board(f + 1, r - 1) and not board.is_occupied(allies, pos8))
            {
                Move mv = Move(p, pos8);
                mv.piece_set(piece);
                if (board.is_occupied(enemies, pos8))
                {
                    m.push_back(mv);
                }
                else
                {
                    m.push_back(mv);
                }
            }
        }

        return m;
    }

    std::vector<Move> Rule::generate_rook_moves(Chessboard chessboard)
    {
        std::vector<Move> m;
        Board board = chessboard.getBoard();
        bool white_turn = chessboard.isWhiteTurn();
        unsigned long long int rook = ((white_turn)
                            ? board.rook_wb : board.rook_bb)->board_get();
        unsigned long long int acc = 0;
        unsigned long long int pos = 0;
        /*std::vector<Position> positions = (chessboard.isWhiteTurn()) ?
                                           board.get_white_rook()
                                           : board.get_black_rook();

        if (chessboard.isWhiteTurn())
        {
            positions = board.get_white_rook();
        }
        else
        {
            positions = board.get_black_rook();
        }

        for (Position p : positions)
        {
            bool white_turn = chessboard.isWhiteTurn();*/
       while (acc < rook) {
            pos = rook ^ (rook & (rook - acc - 1));
            add_xys(pos, m, board, white_turn, PieceType::ROOK);
            /*add_forward(p, m, board, white_turn, PieceType::ROOK);
            add_backward(p, m, board, white_turn, PieceType::ROOK);
            add_leftward(p, m, board, white_turn, PieceType::ROOK);
            add_rightward(p, m, board, white_turn, PieceType::ROOK);*/
            acc |= pos;
        }

        return m;
    }

    std::vector<Move> Rule::generate_queen_moves(Chessboard chessboard)
    {
        std::vector<Move> m;
        Board board = chessboard.getBoard();
        bool white_turn = chessboard.isWhiteTurn();
        unsigned long long int queen = ((white_turn)
                            ? board.queen_wb : board.queen_bb)->board_get();
        unsigned long long int acc = 0;
        unsigned long long int pos = 0;

        while (acc < queen) {
            pos = queen ^ (queen & (queen - acc - 1));

            add_diags(pos, m, board, white_turn, PieceType::QUEEN);
            add_xys(pos, m, board, white_turn, PieceType::QUEEN);
            /*add_forward(move, m, board, white_turn, PieceType::QUEEN);
            add_backward(move, m, board, white_turn, PieceType::QUEEN);
            add_leftward(move, m, board, white_turn, PieceType::QUEEN);
            add_rightward(move, m, board, white_turn, PieceType::QUEEN);*/

            acc |= pos;
        }
        return m;
    }

    std::vector<Move> Rule::generate_bishop_moves(Chessboard chessboard) {
        std::vector<Move> m;
        Board board = chessboard.getBoard();
        bool white_turn = chessboard.isWhiteTurn();
        unsigned long long int bishop = ((white_turn)
                            ? board.bishop_wb : board.bishop_bb)->board_get();
        unsigned long long int acc = 0;
        unsigned long long int pos = 0;

        while (acc < bishop) {
            pos = bishop ^ (bishop & (bishop - acc - 1));
            add_diags(pos, m, board, white_turn, PieceType::BISHOP);
            acc |= pos;
        }
        /*std::vector<Position> bishops = (white_turn) ? board.get_white_bishop()
                            : board.get_black_bishop();
        for (unsigned int i = 0; i < bishops.size(); i++)
            add_diags(bishops[i], m, board, white_turn, PieceType::BISHOP);*/

        return m;
    }

    std::vector<Move> Rule::generate_knight_moves(Chessboard chessboard)
    {
        std::vector<Move> m;
        bool white_turn = chessboard.isWhiteTurn();
        Board board = chessboard.getBoard();
        std::vector<Position> knights = (white_turn) ? board.get_white_knight()
                            : board.get_black_knight();
        auto allies = (white_turn) ? board.white_occupied_board_get()
                            : board.black_occupied_board_get();

        int x;
        int y;
        Position newPos(File::A, Rank::ONE);
        for (Position pos : knights) {
            x = static_cast<int>(pos.file_get());
            y = static_cast<int>(pos.rank_get());
            if (in_board(pos, -2, -1) and not board.is_occupied(allies,
                    (newPos = Position(static_cast<File>(x - 2),
                    static_cast<Rank>(y - 1)))))
            {
                Move mv = Move(pos, newPos);
                opt_piecetype_t opt = board.is_occupied(newPos,
                        (white_turn) ? Color::BLACK : Color::WHITE);
                if (opt.has_value())
                    mv.capture_set(opt.value());
                mv.piece_set(PieceType::KNIGHT);
                m.push_back(mv);
            }
            if (in_board(pos, -2, 1) and not board.is_occupied(allies,
                    (newPos = Position(static_cast<File>(x - 2),
                    static_cast<Rank>(y + 1)))))
            {
                Move mv = Move(pos, newPos);
                opt_piecetype_t opt = board.is_occupied(newPos,
                        (white_turn) ? Color::BLACK : Color::WHITE);
                if (opt.has_value())
                    mv.capture_set(opt.value());
                mv.piece_set(PieceType::KNIGHT);
                m.push_back(mv);
            }
            if (in_board(pos, 2, -1) and not board.is_occupied(allies,
                    (newPos = Position(static_cast<File>(x + 2),
                    static_cast<Rank>(y - 1)))))
            {
                Move mv = Move(pos, newPos);
                opt_piecetype_t opt = board.is_occupied(newPos,
                        (white_turn) ? Color::BLACK : Color::WHITE);
                if (opt.has_value())
                    mv.capture_set(opt.value());
                mv.piece_set(PieceType::KNIGHT);
                m.push_back(mv);
            }
            if (in_board(pos, 2, 1) and not board.is_occupied(allies,
                    (newPos = Position(static_cast<File>(x + 2),
                    static_cast<Rank>(y + 1)))))
            {
                Move mv = Move(pos, newPos);
                opt_piecetype_t opt = board.is_occupied(newPos,
                        (white_turn) ? Color::BLACK : Color::WHITE);
                if (opt.has_value())
                    mv.capture_set(opt.value());
                mv.piece_set(PieceType::KNIGHT);
                m.push_back(mv);
            }
            if (in_board(pos, -1, -2) and not board.is_occupied(allies,
                    (newPos = Position(static_cast<File>(x - 1),
                    static_cast<Rank>(y - 2)))))
            {
                Move mv = Move(pos, newPos);
                opt_piecetype_t opt = board.is_occupied(newPos,
                        (white_turn) ? Color::BLACK : Color::WHITE);
                if (opt.has_value())
                    mv.capture_set(opt.value());
                mv.piece_set(PieceType::KNIGHT);
                m.push_back(mv);

            }
            if (in_board(pos, -1, 2) and not board.is_occupied(allies,
                    (newPos = Position(static_cast<File>(x - 1),
                    static_cast<Rank>(y + 2)))))
            {
                Move mv = Move(pos, newPos);
                opt_piecetype_t opt = board.is_occupied(newPos,
                        (white_turn) ? Color::BLACK : Color::WHITE);
                if (opt.has_value())
                    mv.capture_set(opt.value());
                mv.piece_set(PieceType::KNIGHT);
                m.push_back(mv);

            }
            if (in_board(pos, 1, -2) and not board.is_occupied(allies,
                    (newPos = Position(static_cast<File>(x + 1),
                    static_cast<Rank>(y - 2)))))
            {
                Move mv = Move(pos, newPos);
                opt_piecetype_t opt = board.is_occupied(newPos,
                        (white_turn) ? Color::BLACK : Color::WHITE);
                if (opt.has_value())
                    mv.capture_set(opt.value());
                mv.piece_set(PieceType::KNIGHT);
                m.push_back(mv);

            }
            if (in_board(pos, 1, 2) and not board.is_occupied(allies,
                    (newPos = Position(static_cast<File>(x + 1),
                    static_cast<Rank>(y + 2)))))
            {
                Move mv = Move(pos, newPos);
                opt_piecetype_t opt = board.is_occupied(newPos,
                        (white_turn) ? Color::BLACK : Color::WHITE);
                if (opt.has_value())
                    mv.capture_set(opt.value());
                mv.piece_set(PieceType::KNIGHT);
                m.push_back(mv);
            }
        }
        return m;
    }
}
