#include "chessboard.hh"
#include "rule.hh"

namespace board {
    Chessboard::Chessboard() {
        auto temp = Board();
        board_ = temp;
        white_turn_ = true;
        white_king_castling_ = true;
        white_queen_castling_ = true;
        black_king_castling_ = true;
        black_queen_castling_ = true;
        turn_ = 0;
        last_fifty_turn = 0;
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
            }
        }

        // Fourth line
        if (splited_input[3][0] != '-')
        {
            auto file = splited_input[3][0] - 'a';
            //we want pawn position not where en_passant leads us
            auto rank = splited_input[3][1] - '1';
            Position pos(static_cast<File>(file), static_cast<Rank>(rank));
            en_passant_ = pos;
            en_passant_bitboard_ = pow(2, utils::to_int(pos));
        }
        else
        {
            en_passant_ = nullopt;
            en_passant_bitboard_ = 0;
        }
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
        if (move.piece_get() == PieceType::KING)
        {
            if (white_turn_)
             {
                white_king_castling_ = false;
                white_queen_castling_ = false;
            }
            else
            {
                black_king_castling_ = false;
                black_queen_castling_ = false;
            }
        }
        else if (move.piece_get() == PieceType::ROOK)
        {
            if (move.move_get().first.file_get() == File::A)
            {
                if (white_turn_)
                {
                    white_queen_castling_ = false;
                }
                else
                {
                    black_queen_castling_ = false;
                }
            }
            else if (move.move_get().first.file_get() == File::H)
            {
                if (white_turn_)
                {
                    white_king_castling_ = false;
                }
                else
                {
                    black_king_castling_ = false;
                }
            }
        }
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
            if (is_checkmate(!white_turn_))
            {
                for (auto listener : listeners) {
                    listener->on_player_mat(!white_turn_ ? Color::WHITE
                            : Color::BLACK);
                    listener->on_game_finished();
                }

            }
            else
            {
                for (auto listener : listeners)
                    listener->on_player_check(!white_turn_ ? Color::WHITE
                                                           : Color::BLACK);
            }
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
        en_passant_ = optional<Position>();
        en_passant_bitboard_ = 0;
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

    void Chessboard::delete_king_moves(std::vector<Move>& moves)
    {
        auto cpt = moves.begin();
        for (Move m : moves)
        {
            if (not is_check(isWhiteTurn())
                and m.piece_get() == PieceType::KING and
                not m.king_castling_get() and not m.queen_castling_get())
            {
                moves.erase(cpt);
            }
            cpt++;
        }
    }


    std::vector<Move> Chessboard::generate_legal_moves()
    {
        board_.refresh_occupied();

        Rule rules = Rule();
        std::vector<Move> all_moves;

        rules.generate_pawn_moves(*this, all_moves);
        rules.generate_king_moves(*this, all_moves);
        rules.generate_bishop_moves(*this, all_moves);
        rules.generate_rook_moves(*this, all_moves);
        rules.generate_queen_moves(*this, all_moves);
        rules.generate_knight_moves(*this, all_moves);

        std::vector<Move> res;
        auto save = board_;
        // Put yourself in check part
        for (long unsigned int i = 0; i < all_moves.size(); i++)
        {
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
        if (!perft)
            delete_king_moves(res);

        return res;
    }

    bool Chessboard::is_check(bool color)
    {
        bool check = board_.is_check(color);
        return check;
    }

    bool Chessboard::is_draw()
    {
        return generate_legal_moves().size() == 0;
    }

    bool Chessboard::is_checkmate(bool color)
    {
        bool check = board_.is_check(color);
        if (!check)
            return check;
        white_turn_ = !white_turn_;
        auto moves = generate_legal_moves();
        bool checkmate = true;
        auto save = board_;
        for (auto move : moves)
        {
            auto temp = Board(board_);
            board_ = temp;
            board_.do_move(move, (color ? Color::WHITE : Color::BLACK));
            if (!board_.is_check(color))
            {
                checkmate = false;
                board_ = save;
                break;
            }
            board_ = save;
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
            if (board_.king_wb & bitboard)
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
            else if (board_.queen_wb & bitboard)
                res.piece_set(PieceType::QUEEN);
            else if (board_.knight_wb & bitboard)
                res.piece_set(PieceType::KNIGHT);
            else if (board_.bishop_wb & bitboard)
                res.piece_set(PieceType::BISHOP);
            else if (board_.rook_wb & bitboard)
                res.piece_set(PieceType::ROOK);
            else if (board_.pawn_wb & bitboard)
                res.piece_set(PieceType::PAWN);

            if (board_.king_bb & bitend)
                res.capture_set(PieceType::KING);
            else if (board_.queen_bb & bitend)
                res.capture_set(PieceType::QUEEN);
            else if (board_.knight_bb & bitend)
                res.capture_set(PieceType::KNIGHT);
            else if (board_.bishop_bb & bitend)
                res.capture_set(PieceType::BISHOP);
            else if (board_.rook_bb & bitend)
                res.capture_set(PieceType::ROOK);
            else if (board_.pawn_bb & bitend)
                res.capture_set(PieceType::PAWN);

        }
        else
        {
            if (board_.king_bb & bitboard)
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
            else if (board_.queen_bb & bitboard)
                res.piece_set(PieceType::QUEEN);
            else if (board_.knight_bb & bitboard)
                res.piece_set(PieceType::KNIGHT);
            else if (board_.bishop_bb & bitboard)
                res.piece_set(PieceType::BISHOP);
            else if (board_.rook_bb & bitboard)
                res.piece_set(PieceType::ROOK);
            else if (board_.pawn_bb & bitboard)
                res.piece_set(PieceType::PAWN);

            if (board_.king_wb & bitend)
                res.capture_set(PieceType::KING);
            else if (board_.queen_wb & bitend)
                res.capture_set(PieceType::QUEEN);
            else if (board_.knight_wb & bitend)
                res.capture_set(PieceType::KNIGHT);
            else if (board_.bishop_wb & bitend)
                res.capture_set(PieceType::BISHOP);
            else if (board_.rook_wb & bitend)
                res.capture_set(PieceType::ROOK);
            else if (board_.pawn_wb & bitend)
                res.capture_set(PieceType::PAWN);
        }

        return res;

    }

    void Chessboard::actualise_pgn_move(Move& move)
    {
        if (move.capture_get() != PieceType::NONE)
        {
            auto pow = utils::two_pow(utils::to_int(move.move_get().second));
            if (white_turn_)
            {
                if (board_.pawn_bb & pow)
                    move.capture_set(PieceType::PAWN);
                else if (board_.knight_bb & pow)
                    move.capture_set(PieceType::KNIGHT);
                else if (board_.bishop_bb & pow)
                    move.capture_set(PieceType::BISHOP);
                else if (board_.rook_bb & pow)
                    move.capture_set(PieceType::ROOK);
                else if (board_.queen_bb & pow)
                    move.capture_set(PieceType::QUEEN);
            }
            else
            {
                if (board_.pawn_wb & pow)
                    move.capture_set(PieceType::PAWN);
                else if (board_.knight_wb & pow)
                    move.capture_set(PieceType::KNIGHT);
                else if (board_.bishop_wb & pow)
                    move.capture_set(PieceType::BISHOP);
                else if (board_.rook_wb & pow)
                    move.capture_set(PieceType::ROOK);
                else if (board_.queen_wb & pow)
                    move.capture_set(PieceType::QUEEN);
            }
        }
        else if (move.piece_get() == PieceType::KING)
        {
            if (move.move_get().first.file_get() == File::E)
            {
                if (move.move_get().second.file_get() == File::G)
                    move.king_castling_set(true);

                else if (move.move_get().second.file_get() == File::C)
                    move.queen_castling_set(true);
            }
        }
    }
}
