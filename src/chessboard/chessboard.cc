#include "chessboard.hh"
#include "rule.hh"

namespace board {
    Chessboard::Chessboard() {
        board_ = Board();
    }

    Chessboard::Chessboard(vector<string> splited_input)
    {
        // First line
        board_ = Board(splited_input[0]);

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
            white_turn_ = !white_turn_;
            auto moves = generate_legal_moves();
            if (moves.size() == 0)
            {
                for (auto listener : listeners)
                    listener->on_player_mat(white_turn_ ? Color::WHITE
                                            : Color::BLACK);
            }
            else
            {
                for (auto listener : listeners)
                    listener->on_player_check(white_turn_ ? Color::WHITE
                                            : Color::BLACK);
            }
            white_turn_ = !white_turn_;
        }
        else
        {
            white_turn_ = !white_turn_;
            auto moves = generate_legal_moves();
            if (moves.size() == 0)
            {
                for (auto listener : listeners)
                {
                    listener->on_player_pat(white_turn_ ? Color::WHITE
                                            : Color::BLACK);
                    listener->on_draw();
                }
            }
            white_turn_ = !white_turn_;
        }
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

        std::vector<Move> king = rules.generate_king_moves(*this);
        all_moves = add_in_vector(all_moves, king);

        std::vector<Move> bishops = rules.generate_bishop_moves(*this);
        all_moves = add_in_vector(all_moves, bishops);

        std::vector<Move> rooks = rules.generate_rook_moves(*this);
        all_moves = add_in_vector(all_moves, rooks);

        std::vector<Move> queen = rules.generate_queen_moves(*this);
        all_moves = add_in_vector(all_moves, queen);

        std::vector<Move> knights = rules.generate_knight_moves(*this);
        all_moves = add_in_vector(all_moves, knights);

        std::vector<Move> res;
        // Put yourself in check part
        for (long unsigned int i = 0; i < all_moves.size(); i++)
        {
            auto pair_r = all_moves[i].move_get();
            Move virgin_move = Move(pair_r.first, pair_r.second);
            virgin_move.piece_set(all_moves[i].piece_get());
            Move revert = Move(pair_r.second, pair_r.first);
            revert.piece_set(all_moves[i].piece_get());
            board_.do_move(virgin_move, white_turn_ ? Color::WHITE : Color::BLACK);
            if (!board_.is_check(white_turn_))
            {
                res.push_back(all_moves[i]);
            }
            board_.do_move(revert, white_turn_ ? Color::WHITE : Color::BLACK);
        }

        return res;
    }

    bool Chessboard::is_check()
    {
        bool check = board_.is_check(white_turn_);
        return check;
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
}
