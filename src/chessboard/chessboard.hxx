#pragma once

#include "chessboard.hh"
inline Board Chessboard::getBoard()
{
    return board_;
}

inline void Chessboard::setBoard(Board& board)
{
     board_ = board;
}

inline bool Chessboard::isWhiteTurn()
{
    return white_turn_;
}

inline void Chessboard::setWhiteTurn(bool wt)
{
    white_turn_ = wt;
}

inline bool Chessboard::getWhiteKingCastling()
{
    return white_king_castling_;
}

inline bool Chessboard::getWhiteQueenCastling()
{
    return white_queen_castling_;
}

inline bool Chessboard::getBlackKingCastling()
{
    return black_king_castling_;
}

inline bool Chessboard::getBlackQueenCastling()
{
    return black_queen_castling_;
}

inline std::optional<Position> Chessboard::getEnPassant()
{
    return en_passant_;
}

inline bit Chessboard::getEnPassantBitboard()
{
    return en_passant_bitboard_;
}

inline unsigned int Chessboard::getTurn()
{
    return turn_;
}

inline unsigned int Chessboard::getLastFiftyTurn()
{
    return last_fifty_turn;
}
