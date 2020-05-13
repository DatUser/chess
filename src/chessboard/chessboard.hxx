#pragma once

#include "chessboard.hh"

Board Chessboard::getBoard()
{
    return board_;
}

bool isWhiteTurn()
{
    return white_turn;
}

bool getWhiteKingCastling()
{
    return white_king_castling_;
}

bool getWhiteQueenCastling()
{
    return white_queen_castling_;
}

bool getBlackKingCastling()
{
    return black_king_castling_;
}

bool getBlackQueenCastling()
{
    return black_queen_castling_;
}

Position getEnPassant()
{
    return en_passant_;
}

unsigned int getTurn()
{
    return turn_;
}

unsigned int getLastFiftyTurn()
{
    return last_fifty_turn;
}
