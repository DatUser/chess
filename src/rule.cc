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
        return static_cast<int>(pos.file_get()) + x >= static_cast<int>(File::A) and
                static_cast<int>(pos.file_get()) + x <= static_cast<int>(File::H) and
                static_cast<int>(pos.rank_get()) + y >= static_cast<int>(Rank::ONE) and
                static_cast<int>(pos.rank_get()) + y <= static_cast<int>(Rank::EIGHT);
    }

    void add_diag(Position pos, std::vector<Move>& moves,
                                    Board& board, bool white_turn,
                                    std::pair<int, int>& direction,
                                    PieceType piece) {
        Bitboard ally = (white_turn) ? board.white_occupied_board_get()
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
                not board.is_occupied(&ally,
                (newPos = Position(static_cast<File>(x - i),
                static_cast<Rank>(y - i)))); i++) {

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

    void add_diags(Position pos, std::vector<Move>& moves,
                                    Board& board, bool white_turn,
                                    PieceType piece) {
        std::pair<int, int> direction(-1, -1);
        add_diag(pos, moves, board, white_turn, direction, piece);
        add_diag(pos, moves, board, white_turn,
                            (direction = std::pair<int, int>(1, -1)), piece);
        add_diag(pos, moves, board, white_turn,
                            (direction = std::pair<int, int>(-1, 1)), piece);
       add_diag(pos, moves, board, white_turn,
                            (direction = std::pair<int, int>(1, 1)), piece);
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
        Bitboard ally = (white_turn) ? board.white_occupied_board_get()
                            : board.black_occupied_board_get();
        File file = pos.file_get();
        int f = static_cast<int>(pos.file_get());
        int r = static_cast<int>(pos.rank_get());
        for (int i = 1; i < 8 - r; i++)
        {
            Position new_pos = Position(file, static_cast<Rank>(r + i));
            if (in_board(f, r))
            {
                opt_piecetype_t opt = board.is_occupied(new_pos,
                            (white_turn) ? Color::BLACK : Color::WHITE);
                Move mv = Move(pos, new_pos);
                if (board.is_occupied(&ally, new_pos))
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

    void add_backward(Position pos, std::vector<Move>& moves,
                        Board board, bool white_turn, PieceType piece)
    {
        Bitboard ally = (white_turn) ? board.white_occupied_board_get()
                            : board.black_occupied_board_get();
        File file = pos.file_get();
        int f = static_cast<int>(pos.file_get());
        int r = static_cast<int>(pos.rank_get());
        for (int i = 1; i < 8 - r; i++)
        {
            Position new_pos = Position(file, static_cast<Rank>(r - i));
            if (in_board(f, r))
            {
                opt_piecetype_t opt = board.is_occupied(new_pos,
                            (white_turn) ? Color::BLACK : Color::WHITE);

                Move mv = Move(pos, new_pos);
                if (board.is_occupied(&ally, new_pos))
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
        Bitboard ally = (white_turn) ? board.white_occupied_board_get()
                            : board.black_occupied_board_get();
        Rank rank = pos.rank_get();
        int f = static_cast<int>(pos.file_get());
        int r = static_cast<int>(pos.rank_get());
        for (int i = 1; i < 8; i++)
        {
            Position new_pos = Position(static_cast<File>(f + i), rank);
            if (in_board(f, r))
            {
                opt_piecetype_t opt = board.is_occupied(new_pos,
                            (white_turn) ? Color::BLACK : Color::WHITE);
                Move mv = Move(pos, new_pos);
                if (board.is_occupied(&ally, new_pos))
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
        Bitboard ally = (white_turn) ? board.white_occupied_board_get()
                            : board.black_occupied_board_get();
        Rank rank = pos.rank_get();
        int f = static_cast<int>(pos.file_get());
        int r = static_cast<int>(pos.rank_get());
        for (int i = 1; i < 8; i++)
        {
            Position new_pos = Position(static_cast<File>(f - i), rank);
            if (in_board(f, r))
            {
                opt_piecetype_t opt = board.is_occupied(new_pos,
                            (white_turn) ? Color::BLACK : Color::WHITE);
                Move mv = Move(pos, new_pos);
                if (board.is_occupied(&ally, new_pos))
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
                    mv.capture_set(opt.value());
                    moves.push_back(mv);
                }
            }
            else
            {
                return;
            }
        }
    }

    void pawn_step(std::vector<Move>& moves, Position pos, Board board) {
        int y = static_cast<int>(pos.rank_get()) + 1;
        Position newpos(pos.file_get(), static_cast<Rank>(y));

        if (not board.is_occupied(board.occupied_board_get(), newpos)) {
            Move mv(pos, newpos);
            mv.piece_set(PieceType::PAWN);
            moves.push_back(mv);
        }
    }

    void add_promotion(std::vector<Move>& moves, Position pos, Position newpos) {

        for (int i = 0; i < 4; i++) {
            Move mv(pos, newpos);
            mv.piece_set(PieceType::PAWN);
            mv.promotion_set(piecetype_array[i]);
            moves.push_back(mv);
        }
    }

    //Since it is at the sames place we deal with it in the same funcs
    void pawn_double_step_promotion(std::vector<Move>& moves, Position pos,
            Board board, bool doubl) {
        int y = static_cast<int>(pos.rank_get()) + (doubl) ? 2 : 1;
        Position newpos(pos.file_get(), static_cast<Rank>(y));
        if (not board.is_occupied(board.occupied_board_get(), newpos)) {
            Move mv(pos, newpos);
            mv.piece_set(PieceType::PAWN);
            //GERER LE EN PASSANT
            if (doubl) {
                mv.double_pawn_push_set(true);
                moves.push_back(mv);
            }
             else
                add_promotion(moves, pos, newpos);
        }
    }

    bool pos_equal(Position posA, Position posB) {
        return posA.rank_get() == posB.rank_get() and
                posA.file_get() == posB.file_get();
    }

    void pawn_eat(std::vector<Move>& moves, Position pos, Chessboard chessboard,
                    bool white_turn) {
        int x_r = static_cast<int>(pos.file_get()) + 1;
        int x_l = static_cast<int>(pos.file_get()) - 1;
        int y = static_cast<int>(pos.rank_get()) + 1;

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

        if (in_board(pos, -1, 1)) {
            if (opt_l.has_value()) {
                Move mv(pos, left);
                mv.piece_set(PieceType::PAWN);
                mv.capture_set(opt_l.value());
                moves.push_back(mv);
            }
        }

        if (in_board(pos, 1, 1)) {
            if (opt_r.has_value()) {
                Move mv(pos, right);
                mv.piece_set(PieceType::PAWN);
                mv.capture_set(opt_r.value());
                moves.push_back(mv);
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
    }

    std::vector<Move> Rule::generate_pawn_moves(Chessboard chessboard)
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
                pawn_step(m, pawns[i], board);
                pawn_double_step_promotion(m, pawns[i], board, white_turn);
                pawn_eat(m, pawns[i], chessboard, white_turn);
                break;

            case Rank::SEVEN:
                pawn_step(m, pawns[i], board);
                pawn_double_step_promotion(m, pawns[i], board, not white_turn);
                pawn_eat(m, pawns[i], chessboard, white_turn);
                break;

            default:
                pawn_step(m, pawns[i], board);
                pawn_eat(m, pawns[i], chessboard, white_turn);
                break;
            }
        }

        return m;
    }

    void check_king_castling(std::vector<Move>& moves,
                        Position pos, bool white_turn, Board board)
    {
        Bitboard ally = (white_turn) ? board.white_occupied_board_get()
                            : board.black_occupied_board_get();

        Bitboard enemy = (white_turn) ? board.black_occupied_board_get()
                            : board.white_occupied_board_get();


        int f = static_cast<int>(pos.file_get());
        int r = static_cast<int>(pos.rank_get());
        Rank rank = pos.rank_get();
        for (int i = f + 1; in_board(i, r); i++)
        {
            Position new_pos = Position(static_cast<File>(i), rank);
            Move mv = Move(pos, new_pos);
            if (board.is_occupied(&enemy, new_pos))
            {
                return;
            }

            if (board.is_occupied(&ally, new_pos))
            {
                opt_piecetype_t opt = board.is_occupied(new_pos,
                            (white_turn) ? Color::BLACK : Color::WHITE);
                if (opt.value() == PieceType::ROOK)
                {
                    mv.king_castling_set(true);
                    moves.push_back(mv);
                    return;
                }
            }
        }
    }


    void check_queen_castling(std::vector<Move>& moves,
                        Position pos, bool white_turn, Board board)
    {
        Bitboard ally = (white_turn) ? board.white_occupied_board_get()
                            : board.black_occupied_board_get();

        Bitboard enemy = (white_turn) ? board.black_occupied_board_get()
                            : board.white_occupied_board_get();


        int f = static_cast<int>(pos.file_get());
        int r = static_cast<int>(pos.rank_get());
        Rank rank = pos.rank_get();
        for (int i = f - 1; in_board(i, r); i--)
        {
            Position new_pos = Position(static_cast<File>(i), rank);
            Move mv = Move(pos, new_pos);
            if (board.is_occupied(&enemy, new_pos))
            {
                return;
            }

            if (board.is_occupied(&ally, new_pos))
            {
                opt_piecetype_t opt = board.is_occupied(new_pos,
                            (white_turn) ? Color::BLACK : Color::WHITE);
                if (opt.value() == PieceType::ROOK)
                {
                    mv.queen_castling_set(true);
                    moves.push_back(mv);
                    return;
                }
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

        Bitboard allies = (chessboard.isWhiteTurn()) ?
                           board.white_occupied_board_get()
                           : board.black_occupied_board_get();

        Bitboard enemies = (chessboard.isWhiteTurn()) ?
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

        for (Position p : positions)
        {
            if (chessboard.getWhiteKingCastling() or chessboard.getBlackKingCastling())
            {
                check_king_castling(m, p, chessboard.isWhiteTurn(), board);
            }
            if (chessboard.getWhiteQueenCastling() or chessboard.getBlackQueenCastling())
            {
                check_queen_castling(m, p, chessboard.isWhiteTurn(), board);
            }

            File file = p.file_get();
            Rank rank = p.rank_get();
            int f = static_cast<int>(p.file_get());
            int r = static_cast<int>(p.rank_get());

            Position pos1 = Position(static_cast<File>(f - 1), static_cast<Rank>(r + 1));
            Position pos2 = Position(file, static_cast<Rank>(r + 1));
            Position pos3 = Position(static_cast<File>(f + 1), static_cast<Rank>(r + 1));
            Position pos4 = Position(static_cast<File>(f - 1), rank);
            Position pos5 = Position(static_cast<File>(f + 1), rank);
            Position pos6 = Position(static_cast<File>(f - 1), static_cast<Rank>(r - 1));
            Position pos7 = Position(file, static_cast<Rank>(r - 1));
            Position pos8 = Position(static_cast<File>(f + 1), static_cast<Rank>(r - 1));

            PieceType piece = PieceType::KING;

            if (in_board(f - 1, r + 1) and not board.is_occupied(&allies, pos1))
            {
                Move mv = Move(p, pos1);
                if (board.is_occupied(&enemies, pos1))
                {
                    mv.piece_set(piece);
                    m.push_back(mv);
                }
                else
                {
                    m.push_back(mv);
                }
            }
            if (in_board(f, r + 1) and not board.is_occupied(&allies, pos2))
            {
                Move mv = Move(p, pos2);
                if (board.is_occupied(&enemies, pos2))
                {
                    mv.piece_set(piece);
                    m.push_back(mv);
                }
                else
                {
                    m.push_back(mv);
                }
            }
            if (in_board(f + 1, r + 1) and not board.is_occupied(&allies, pos3))
            {
                Move mv = Move(p, pos3);
                if (board.is_occupied(&enemies, pos3))
                {
                    mv.piece_set(piece);
                    m.push_back(mv);
                }
                else
                {
                    m.push_back(mv);
                }
            }
            if (in_board(f - 1, r) and not board.is_occupied(&allies, pos4))
            {
                Move mv = Move(p, pos4);
                if (board.is_occupied(&enemies, pos4))
                {
                    mv.piece_set(piece);
                    m.push_back(mv);
                }
                else
                {
                    m.push_back(mv);
                }
            }
            if (in_board(f + 1, r) and not board.is_occupied(&allies, pos5))
            {
                Move mv = Move(p, pos5);
                if (board.is_occupied(&enemies, pos5))
                {
                    mv.piece_set(piece);
                    m.push_back(mv);
                }
                else
                {
                    m.push_back(mv);
                }
            }
            if (in_board(f - 1, r - 1) and not board.is_occupied(&allies, pos6))
            {
                Move mv = Move(p, pos6);
                if (board.is_occupied(&enemies, pos6))
                {
                    mv.piece_set(piece);
                    m.push_back(mv);
                }
                else
                {
                    m.push_back(mv);
                }
            }
            if (in_board(f, r - 1) and not board.is_occupied(&allies, pos7))
            {
                Move mv = Move(p, pos7);
                if (board.is_occupied(&enemies, pos7))
                {
                    mv.piece_set(piece);
                    m.push_back(mv);
                }
                else
                {
                    m.push_back(mv);
                }
            }
            if (in_board(f + 1, r - 1) and not board.is_occupied(&allies, pos8))
            {
                Move mv = Move(p, pos8);
                if (board.is_occupied(&enemies, pos8))
                {
                    mv.piece_set(piece);
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
        std::vector<Position> positions = (chessboard.isWhiteTurn()) ?
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
            bool white_turn = chessboard.isWhiteTurn();
            add_forward(p, m, board, white_turn, PieceType::ROOK);
            add_backward(p, m, board, white_turn, PieceType::ROOK);
            add_leftward(p, m, board, white_turn, PieceType::ROOK);
            add_rightward(p, m, board, white_turn, PieceType::ROOK);
        }

        return m;
    }

    std::vector<Move> Rule::generate_queen_moves(Chessboard chessboard)
    {
        std::vector<Move> m;
        Board board = chessboard.getBoard();
        bool white_turn = chessboard.isWhiteTurn();
        std::vector<Position> queen = (white_turn) ? board.get_white_queen()
                            : board.get_black_queen();
        for (unsigned int i = 0; i < queen.size(); i++) {
            add_diags(queen[i], m, board, white_turn, PieceType::QUEEN);
            add_forward(queen[i], m, board, white_turn, PieceType::QUEEN);
            add_backward(queen[i], m, board, white_turn, PieceType::QUEEN);
            add_leftward(queen[i], m, board, white_turn, PieceType::QUEEN);
            add_rightward(queen[i], m, board, white_turn, PieceType::QUEEN);
        }
        return m;
    }

    std::vector<Move> Rule::generate_bishop_moves(Chessboard chessboard) {
        std::vector<Move> m;
        Board board = chessboard.getBoard();
        bool white_turn = chessboard.isWhiteTurn();
        std::vector<Position> bishops = (white_turn) ? board.get_white_bishop()
                            : board.get_black_bishop();
        for (unsigned int i = 0; i < bishops.size(); i++)
            add_diags(bishops[i], m, board, white_turn, PieceType::BISHOP);

        return m;
    }

    std::vector<Move> Rule::generate_knight_moves(Chessboard chessboard)
    {
        std::vector<Move> m;
        bool white_turn = chessboard.isWhiteTurn();
        Board board = chessboard.getBoard();
        std::vector<Position> knights = (white_turn) ? board.get_white_knight()
                            : board.get_black_knight();
        Bitboard allies = (white_turn) ? board.white_occupied_board_get()
                            : board.black_occupied_board_get();

        int x;
        int y;
        Position newPos(File::A, Rank::ONE);

        for (Position pos : knights) {
            x = static_cast<int>(pos.file_get());
            y = static_cast<int>(pos.rank_get());
            for (int i = -2; i <= 2; i++) {
                i = (i == 0) ? i + 1 : i;
                for (int j = -2; j <= 2; j++) {
                    j = (j == 0) ? j + 1 : j;
                    if (i != j and in_board(pos, i, j) and not
                        board.is_occupied(&allies,
                        (newPos = Position(static_cast<File>(x + i),
                                            static_cast<Rank>(y + j))))) {
                        Move mv = Move(pos, newPos);
                        opt_piecetype_t opt = board.is_occupied(newPos,
                            (white_turn) ? Color::BLACK : Color::WHITE);
                        if (opt.has_value())
                            mv.capture_set(opt.value());
                        mv.piece_set(PieceType::KNIGHT);
                        m.push_back(mv);
                    }
                }
            }
        }

        return m;
    }
}
