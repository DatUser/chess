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

        king_bb = Bitboard(PieceType::KING, Color::BLACK);
        queen_bb = Bitboard(PieceType::QUEEN, Color::BLACK);
        knight_bb = Bitboard(PieceType::KNIGHT, Color::BLACK);
        bishop_bb = Bitboard(PieceType::BISHOP, Color::BLACK);
        rook_bb = Bitboard(PieceType::ROOK, Color::BLACK);
        pawn_bb = Bitboard(PieceType::PAWN, Color::BLACK);

        occupied_board = Bitboard();

        white_danger = Bitboard();
        black_danger = Bitboard();
    }

    void Board::refresh_occupied()
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

        occupied_board.print();
    }

    void Board::compute_danger() {
        // First refresh the occupied bitboard
        refresh_occupied();
        compute_white_danger();
        compute_black_danger();
    }

    void Board::compute_white_danger() {
        /*int king_power = utils::pow_two(king_bb.board_get());
        int queen_power = utils::pow_two(queen_bb.board_get());
        int bishop_power = utils::pow_two(bishop_bb.board_get());
        int knight_power = utils::pow_two(knight_bb.board_get());
        int rook_power = utils::pow_two(rook_bb.board_get());
        int pawn_power = utils::pow_two(pawn_bb.board_get());*/
    }

    void Board::compute_black_danger() {
        /*int king_power = utils::pow_two(king_wb.board_get());
        int queen_power = utils::pow_two(queen_wb.board_get());
        int bishop_power = utils::pow_two(bishop_wb.board_get());
        int knight_power = utils::pow_two(knight_wb.board_get());
        int rook_power = utils::pow_two(rook_wb.board_get());
        int pawn_power = utils::pow_two(pawn_wb.board_get());*/
    }

    void is_occupied(Position position) {
        position = position;
    }
}
