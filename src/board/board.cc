#include "board.hh"

namespace board {

    Board::Board()
    {
        king_wb = Bitboard(PieceType::KING, Color::WHITE);
        queen_wb = Bitboard(PieceType::QUEEN, Color::WHITE);
        knight_wb = Bitboard(PieceType::KNIGHT, Color::WHITE);
        bishop_wb = Bitboard(PieceType::BISHOP, Color::WHITE);
        rook_wb = Bitboard(PieceType::ROOK, Color::WHITE);
        pawn_wb = Bitboard(PieceType::PAWN, Color::WHITE);

        king_wb = Bitboard(PieceType::KING, Color::BLACK);
        queen_wb = Bitboard(PieceType::QUEEN, Color::BLACK);
        knight_wb = Bitboard(PieceType::KNIGHT, Color::BLACK);
        bishop_wb = Bitboard(PieceType::BISHOP, Color::BLACK);
        rook_wb = Bitboard(PieceType::ROOK, Color::BLACK);
        pawn_wb = Bitboard(PieceType::PAWN, Color::BLACK);

        occupied_board = Bitboard();

        white_danger = Bitboard();
        black_danger = Bitboard();
    }

    void refresh_occupied()
    {
        occupied_board |= king_wb;
        occupied_board |= queen_wb;
        occupied_board |= knight_wb;
        occupied_board |= bishop_wb;
        occupied_board |= rook_wb;
        occupied_board |= pawn_wb;

        occupied_board |= king_bb;
        occupied_board |= queen_bb;
        occupied_board |= knight_bb;
        occupied_board |= bishop_bb;
        occupied_board |= rook_bb;
        occupied_board |= pawn_bb;
    }
}
