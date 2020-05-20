#include "chessboard.hh"
#include "rule.hh"

namespace board {
    Chessboard::Chessboard() {
        auto temp = Board();
        board_ = temp;
    }

    Chessboard::Chessboard(vector<string> splited_input)
    {
        // First line
        auto temp = Board(splited_input[0]);
        board_ = temp;

        // Second line
        if (splited_input[1][0] == 'w')
            white_turn_ = true;
        else
            white_turn_ = false;

        // Third line
        for (long unsigned int i = 0; i < splited_input[2].size(); i++)
        {
            switch (splited_input[2][i])
            {
                case 'K':
                    white_king_castling_ = true;
                    break;
                case 'Q':
                    white_queen_castling_ = true;
                    break;
                case 'k':
                    black_king_castling_ = true;
                    break;
                case 'q':
                    black_queen_castling_ = true;
                    break;
                default:
                    white_king_castling_ = false;
                    white_queen_castling_ = false;
                    black_king_castling_ = false;
                    black_queen_castling_  = false;
            }
        }

        // Fourth line
        /*if (splited_input[3][0] != '-')
        {
            auto file = splited_input[3][0] - 'a';
            auto rank = splited_input[3][1] - '0';
        }*/
    }

    ChessboardInterface::opt_piece_t
            Chessboard::operator[](const Position& position) const
    {
        auto piece = board_.is_occupied(position, Color::WHITE);
        if (piece.has_value())
            return pair<PieceType, Color>(piece.value(), Color::WHITE);
        return pair<PieceType, Color>(board_.is_occupied(position,
                                      Color::BLACK).value(), Color::BLACK);
    }

    void Chessboard::do_move(Move move)
    {
        auto listeners = listener::ListenerManager::instance().listeners_get();
        for (auto listener : listeners)
            listener->on_piece_moved(move.piece_get(), move.move_get().first,
                                                       move.move_get().second);
        board_.do_move(move, white_turn_ ? Color::WHITE : Color::BLACK);
        if (move.capture_get() != PieceType::NONE)
        {
            for (auto listener : listeners)
                listener->on_piece_taken(move.capture_get(),
                                         move.move_get().second);
        }
        if (move.promotion_get() != PieceType::NONE)
        {
            for (auto listener : listeners)
                listener->on_piece_promoted(move.promotion_get(),
                            move.move_get().second);
        }
        else if (move.king_castling_get())
        {
            for (auto listener : listeners)
                listener->on_kingside_castling(white_turn_ ? Color::WHITE
                                            : Color::BLACK);
        }
        else if (move.queen_castling_get())
        {
            for (auto listener : listeners)
                listener->on_queenside_castling(white_turn_ ? Color::WHITE
                                            : Color::BLACK);

        }
        if (board_.is_check(!white_turn_))
        {
            for (auto listener : listeners)
                listener->on_player_check(!white_turn_ ? Color::WHITE
                                                       : Color::BLACK);

        }
        else if (board_.is_check(!white_turn_))
        {
            // Need update, not accurate at all
            for (auto listener : listeners)
                listener->on_player_check(!white_turn_ ? Color::WHITE
                                                       : Color::BLACK);
        }
        else
        {
            auto moves = generate_legal_moves();
            if (moves.size() == 0)
            {
                for (auto listener : listeners)
                {
                    listener->on_player_pat(!white_turn_ ? Color::WHITE
                                            : Color::BLACK);
                    listener->on_draw();
                }
            }
        }
        white_turn_ = not white_turn_;
    }

    std::vector<Move> add_in_vector(std::vector<Move> v1, std::vector<Move> v2)
    {
        for (Move mv : v2)
        {
            v1.push_back(mv);
        }
        return v1;
    }


    std::vector<Move> Chessboard::generate_legal_moves()
    {
        board_.refresh_occupied();

        Rule rules = Rule();
        std::vector<Move> all_moves;

        std::vector<Move> pawns = rules.generate_pawn_moves(*this);
        all_moves = add_in_vector(all_moves, pawns);
        //std::cout << "pawns moves: " << all_moves.size() << "\n";

        std::vector<Move> king = rules.generate_king_moves(*this);
        all_moves = add_in_vector(all_moves, king);
        //std::cout << "king moves: " << all_moves.size() << "\n";

        std::vector<Move> bishops = rules.generate_bishop_moves(*this);
        all_moves = add_in_vector(all_moves, bishops);
        //std::cout << "bishops moves: " << all_moves.size() << "\n";

        std::vector<Move> rooks = rules.generate_rook_moves(*this);
        all_moves = add_in_vector(all_moves, rooks);
        //std::cout << "rooks moves: " << all_moves.size() << "\n";

        std::vector<Move> queen = rules.generate_queen_moves(*this);
        all_moves = add_in_vector(all_moves, queen);
        //std::cout << "queen moves: " << all_moves.size() << "\n";

        std::vector<Move> knights = rules.generate_knight_moves(*this);
        all_moves = add_in_vector(all_moves, knights);
        //std::cout << "knights moves: " << all_moves.size() << "\n";

        std::vector<Move> res;
        // Put yourself in check part
        for (long unsigned int i = 0; i < all_moves.size(); i++)
        {
            auto save = board_;
            auto temp = Board(board_);
            setBoard(temp);
            board_.do_move(all_moves[i], white_turn_ ? Color::WHITE
                                                     : Color::BLACK);
            if (!board_.is_check(white_turn_))
            {
                res.push_back(all_moves[i]);
            }
            setBoard(save);
        }

        return res;
    }

    bool Chessboard::is_check()
    {
        bool check = board_.is_check(!white_turn_);
        return check;
    }

    bool Chessboard::is_draw()
    {
        return generate_legal_moves().size() == 0;
    }

    bool Chessboard::is_checkmate()
    {
        bool check = board_.is_check(!white_turn_);
        if (!check)
            return check;

        white_turn_ = !white_turn_;
        auto moves = generate_legal_moves();
        bool checkmate = true;
        for (auto move : moves)
        {
            auto pair_r = move.move_get();
            Move virgin_move = Move(pair_r.first, pair_r.second);
            virgin_move.piece_set(move.piece_get());
            Move revert = Move(pair_r.second, pair_r.first);
            revert.piece_set(move.piece_get());
            board_.do_move(virgin_move, white_turn_ ? Color::WHITE
                    : Color::BLACK);
            if (!board_.is_check(white_turn_))
            {
                checkmate = false;
                board_.do_move(revert, white_turn_ ? Color::WHITE
                                                   : Color::BLACK);
                break;
            }
            board_.do_move(revert, white_turn_ ? Color::WHITE
                                               : Color::BLACK);
        }
        white_turn_ = !white_turn_;
        return checkmate;
    }

    void Chessboard::print()
    {
        board_.print();
    }

    bool Chessboard::is_move_legal(Move move)
    {
        std::vector<Move> moves = generate_legal_moves();
        for (Move m : moves)
        {
            if (m == move)
            {
                return true;
            }
        }
        return false;
    }

    Move Chessboard::to_move(string move)
    {
        int bfile = move[0] - 'a';
        int brank = move[1] - '1';
        int efile = move[2] - 'a';
        int erank = move[3] - '1';

        auto begin = board::Position((board::File) bfile, (board::Rank) brank);
        auto end = board::Position((board::File) efile, (board::Rank) erank);

        auto res = Move(begin, end);
        if (move.size() == 5)
        {
            switch (move[4])
            {
                case 'q':
                    res.promotion_set(PieceType::QUEEN);
                    break;
                case 'b':
                    res.promotion_set(PieceType::BISHOP);
                    break;
                case 'k':
                    res.promotion_set(PieceType::KNIGHT);
                    break;
                case 'r':
                    res.promotion_set(PieceType::ROOK);
                    break;
                default:
                    break;
            }
        }
        auto bitboard = utils::two_pow(utils::to_int(begin));
        auto bitend = utils::two_pow(utils::to_int(end));
        if (isWhiteTurn())
        {
            if (getBoard().king_wb->board_get() & bitboard)
            {
                res.piece_set(PieceType::KING);
                if (bitboard << 2 == bitend)
                {
                    res.queen_castling_set(true);
                }
                if (bitboard >> 2 == bitend)
                {
                    res.king_castling_set(true);
                }
            }
            else if (getBoard().queen_wb->board_get() & bitboard)
                res.piece_set(PieceType::QUEEN);
            else if (getBoard().knight_wb->board_get() & bitboard)
                res.piece_set(PieceType::KNIGHT);
            else if (getBoard().bishop_wb->board_get() & bitboard)
                res.piece_set(PieceType::BISHOP);
            else if (getBoard().rook_wb->board_get() & bitboard)
                res.piece_set(PieceType::ROOK);
            else if (getBoard().pawn_wb->board_get() & bitboard)
                res.piece_set(PieceType::PAWN);

            if (getBoard().king_bb->board_get() & bitend)
                res.capture_set(PieceType::KING);
            else if (getBoard().queen_bb->board_get() & bitend)
                res.capture_set(PieceType::QUEEN);
            else if (getBoard().knight_bb->board_get() & bitend)
                res.capture_set(PieceType::KNIGHT);
            else if (getBoard().bishop_bb->board_get() & bitend)
                res.capture_set(PieceType::BISHOP);
            else if (getBoard().rook_bb->board_get() & bitend)
                res.capture_set(PieceType::ROOK);
            else if (getBoard().pawn_bb->board_get() & bitend)
                res.capture_set(PieceType::PAWN);

        }
        else
        {
            if (getBoard().king_bb->board_get() & bitboard)
            {
                res.piece_set(PieceType::KING);
                res.piece_set(PieceType::KING);
                if (bitboard << 2 == bitend)
                {
                    res.queen_castling_set(true);
                }
                if (bitboard >> 2 == bitend)
                {
                    res.king_castling_set(true);
                }
            }
            else if (getBoard().queen_bb->board_get() & bitboard)
                res.piece_set(PieceType::QUEEN);
            else if (getBoard().knight_bb->board_get() & bitboard)
                res.piece_set(PieceType::KNIGHT);
            else if (getBoard().bishop_bb->board_get() & bitboard)
                res.piece_set(PieceType::BISHOP);
            else if (getBoard().rook_bb->board_get() & bitboard)
                res.piece_set(PieceType::ROOK);
            else if (getBoard().pawn_bb->board_get() & bitboard)
                res.piece_set(PieceType::PAWN);

            if (getBoard().king_wb->board_get() & bitend)
                res.capture_set(PieceType::KING);
            else if (getBoard().queen_wb->board_get() & bitend)
                res.capture_set(PieceType::QUEEN);
            else if (getBoard().knight_wb->board_get() & bitend)
                res.capture_set(PieceType::KNIGHT);
            else if (getBoard().bishop_wb->board_get() & bitend)
                res.capture_set(PieceType::BISHOP);
            else if (getBoard().rook_wb->board_get() & bitend)
                res.capture_set(PieceType::ROOK);
            else if (getBoard().pawn_wb->board_get() & bitend)
                res.capture_set(PieceType::PAWN);
        }

        return res;

    }
}
