#pragma once

#include "chessboard.hh"
Board Chessboard::getBoard()
{
    return board_;
}

bool Chessboard::isWhiteTurn()
{
    return white_turn_;
}

bool Chessboard::getWhiteKingCastling()
{
    return white_king_castling_;
}

bool Chessboard::getWhiteQueenCastling()
{
    return white_queen_castling_;
}

bool Chessboard::getBlackKingCastling()
{
    return black_king_castling_;
}

bool Chessboard::getBlackQueenCastling()
{
    return black_queen_castling_;
}

Position Chessboard::getEnPassant()
{
    return en_passant_.value();
}

unsigned int Chessboard::getTurn()
{
    return turn_;
}

unsigned int Chessboard::getLastFiftyTurn()
{
    return last_fifty_turn;
}
