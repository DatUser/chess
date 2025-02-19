#pragma once

#include "../all.hh"
#include "../chessboard-interface.hh"
#include "../listener/listener-manager.hh"
#include "../utils/all.hh"
#include "../board/all.hh"

using namespace std;

namespace board {
    class Chessboard : public ChessboardInterface {
    public:
        using side_piece_t = pair<PieceType, Color>;
        using opt_piece_t = optional<side_piece_t>;

        Chessboard();
        Chessboard(vector<string> splited_input);

        std::vector<Move> generate_legal_moves();
        void do_move(Move move);
        bool is_move_legal(Move move);
        bool is_check(bool color);
        bool is_checkmate(bool color);
        bool is_draw();
        std::pair<PieceType, Color> operator[](Position position);
        void actualise_pgn_move(Move& move);

        Board getBoard();
        void setBoard(Board& board);
        bool isWhiteTurn();
        void setWhiteTurn(bool wt);
        bool getWhiteKingCastling();
        bool getWhiteQueenCastling();
        bool getBlackKingCastling();
        bool getBlackQueenCastling();
        std::optional<Position> getEnPassant();
        unsigned long long int getEnPassantBitboard();

        unsigned int getTurn();
        unsigned int getLastFiftyTurn();

        opt_piece_t operator[](const Position& position) const override;

        // DEBUG TIER
        void print();

        Move to_move(string move);

        void delete_king_moves(std::vector<Move>& moves);

        bool perft = false;

    private:
        Board board_;
        bool white_turn_;
        bool white_king_castling_ = false;
        bool white_queen_castling_ = false;
        bool black_king_castling_ = false;
        bool black_queen_castling_ = false;
        optional<Position> en_passant_;
        unsigned long long int en_passant_bitboard_;
        unsigned int turn_;
        unsigned int last_fifty_turn;
    };
    #include "chessboard.hxx"
}

#include "chessboard.hxx"
