#pragma once

#include "chessboard.hh"
inline Board Chessboard::getBoard()
{
    return board_;
}

inline bool Chessboard::isWhiteTurn()
{
    return white_turn_;
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

inline unsigned int Chessboard::getTurn()
{
    return turn_;
}

inline unsigned int Chessboard::getLastFiftyTurn()
{
    return last_fifty_turn;
}
