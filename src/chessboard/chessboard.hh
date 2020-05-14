#pragma once

#include "../all.hh"
#include "../utils/all.hh"
#include "../board/all.hh"
#include "../bitboard/all.hh"

using namespace std;

namespace board {
    class Chessboard {
    public:
        Chessboard();
        Chessboard(vector<string> splited_input);

        std::vector<Move> generate_legal_moves();
        void do_move(Move move);
        bool is_move_legal(Move move);
        bool is_check();
        bool is_checkmate();
        bool is_draw();
        std::pair<PieceType, Color> operator[](Position position);

        Board getBoard();
        bool isWhiteTurn();
        bool getWhiteKingCastling();
        bool getWhiteQueenCastling();
        bool getBlackKingCastling();
        bool getBlackQueenCastling();
        std::optional<Position> getEnPassant();
        unsigned int getTurn();
        unsigned int getLastFiftyTurn();
        // DEBUG TIER
        void print();

    private:
        Board board_;
        bool white_turn_;
        bool white_king_castling_;
        bool white_queen_castling_;
        bool black_king_castling_;
        bool black_queen_castling_;
        optional<Position> en_passant_;
        unsigned int turn_;
        unsigned int last_fifty_turn;
    };
    #include "chessboard.hxx"
}
