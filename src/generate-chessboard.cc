#include "generate-chessboard.hh"

namespace board
{
    std::vector<Move> get_moves_from_pgn(const std::string& file)
    {
        std::vector<PgnMove> pgn_moves = pgn_parser::parse_pgn(file);

        std::vector<Move> result;

        for (PgnMove pm : pgn_moves)
        {
            Move mv = Move(pm.get_start(), pm.get_end());
            mv.piece_set(pm.get_piece());
            if (pm.get_promotion() != std::nullopt)
            {
                mv.promotion_set(pm.get_promotion().value());
            }
            if (pm.get_capture())
            {
                mv.capture_set(pm.get_piece());
            }

            result.push_back(mv);
        }

        return result;
    }

    Chessboard generate_chessboard(std::vector<Move> moves)
    {
        auto king_wb = Bitboard(PieceType::KING, Color::WHITE);
                auto queen_wb = Bitboard(PieceType::QUEEN, Color::WHITE);
                auto knight_wb = Bitboard(PieceType::KNIGHT, Color::WHITE);
                auto bishop_wb = Bitboard(PieceType::BISHOP, Color::WHITE);
                auto rook_wb = Bitboard(PieceType::ROOK, Color::WHITE);
                auto pawn_wb = Bitboard(PieceType::PAWN, Color::WHITE);

                auto king_bb = Bitboard(PieceType::KING, Color::BLACK);
                auto queen_bb = Bitboard(PieceType::QUEEN, Color::BLACK);
                auto knight_bb = Bitboard(PieceType::KNIGHT, Color::BLACK);
                auto bishop_bb = Bitboard(PieceType::BISHOP, Color::BLACK);
                auto rook_bb = Bitboard(PieceType::ROOK, Color::BLACK);
                auto pawn_bb = Bitboard(PieceType::PAWN, Color::BLACK);

                auto occupied_board = Bitboard();

                auto white_occupied_board = Bitboard();
                auto black_occupied_board = Bitboard();

                auto white_danger = Bitboard();
                auto black_danger = Bitboard();

                auto board = Board(
                        king_wb,
                        queen_wb,
                        knight_wb,
                        bishop_wb,
                        rook_wb,
                        pawn_wb,
                        king_bb,
                        queen_bb,
                        knight_bb,
                        bishop_bb,
                        rook_bb,
                        pawn_bb,
                        white_occupied_board,
                        black_occupied_board,
                        occupied_board,
                        white_danger,
                        black_danger
                        );
        Chessboard chessboard = Chessboard(board);
        int i = 0;
        for (Move m : moves)
        {
            chessboard.setWhiteTurn((i % 2 == 0) ? true : false);
            chessboard.do_move(m);
            i++;
        }

        return chessboard;
    }
}
