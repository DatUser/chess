#include "chessboard.hh"

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
        
    }

    void Chessboard::do_move(Move move)
    {
        if (white_turn_)
            board_.do_move(move, Color::WHITE);
        else
            board_.do_move(move, Color::BLACK);
    }

    bool Chessboard::is_check()
    {
        return board_.is_check(white_turn_);
    }
}
