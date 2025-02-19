#include "ai.hh"
#include <climits>

#define CENTER 1099494850560
#define QUEEN_WT 1000
#define ROOK_WT 200
#define KNIGHT_WT 150
#define BISHOP_WT 200
#define PAWN_WT 10

#define RFC_BOARD_1 35538699412471296
#define RFC_BOARD_2 66125924401152
#define RFC_BOARD_3 103481868288

#define BABEL_WHITE 996029693952
#define BABEL_BLACK 281371478065152

namespace chess_engine {

    int evaluate_white(Chessboard& board);
    int evaluate_black(Chessboard& board);
    int rec_search(Chessboard& board, int depth, Move move,
                                                bool maxmin, int max, int min);

    //piece is board state after mask
    int worth_pos(unsigned long long int piece, int weight, int num, int den) {
        int total = 0;
        unsigned long long int acc = 0;
        while (acc != piece) {
            total += weight * num / den;
            acc |= piece ^ (piece & (piece - acc - 1));
        }

        return total;
    }

    int evaluate(Chessboard& chessboard)
    {
        if (chessboard.is_checkmate(chessboard.isWhiteTurn()))
            return 999999;
        if (chessboard.is_draw())
            return 0;
        if (chessboard.isWhiteTurn())
            return evaluate_white(chessboard);
        return evaluate_black(chessboard);
    }

    int evaluate_white(Chessboard& chessboard)
    {
        auto board = chessboard.getBoard();
        int res = 0;
        auto black_p = board.get_black_pawn();
        auto black_q = board.get_black_queen();
        auto black_b = board.get_black_bishop();
        auto black_r = board.get_black_rook();
        auto black_k = board.get_black_knight();

        // Calculating queens impact
        auto pieces = board.get_white_queen();
        res += (pieces.size() - black_q.size()) * QUEEN_WT
                + worth_pos(RFC_BOARD_1 & board.queen_wb, QUEEN_WT, 1, 4)
                + worth_pos(RFC_BOARD_2 & board.queen_wb, QUEEN_WT, 1, 3)
                + worth_pos(RFC_BOARD_3 & board.queen_wb, QUEEN_WT, 1, 2);

        // Calculating rooks impact, increasing as pawns disappear
        pieces = board.get_white_rook();
        res += (pieces.size() - black_r.size())
                * (ROOK_WT - 5 * black_p.size())
                + worth_pos(BABEL_WHITE & board.rook_wb, ROOK_WT, 1, 2);

        // Calculating knights impact, decreasing as pawns disappear
        pieces = board.get_white_pawn();
        res += (board.get_white_knight().size() - black_k.size())
            * (KNIGHT_WT - 2 * (black_p.size() + pieces.size()))
                + worth_pos(RFC_BOARD_1 & board.knight_wb, KNIGHT_WT, 1, 4)
                + worth_pos(RFC_BOARD_2 & board.knight_wb, KNIGHT_WT, 1, 3)
                + worth_pos(RFC_BOARD_3 & board.knight_wb, KNIGHT_WT, 1, 2);

        // Calculating pawn impact
        res += (pieces.size() - black_p.size()) * PAWN_WT;

        // Calculating bishop impact
        pieces = board.get_white_bishop();
        res += (pieces.size() - black_b.size()) * BISHOP_WT
                + worth_pos(RFC_BOARD_1 & board.bishop_wb, BISHOP_WT, 1, 4)
                + worth_pos(RFC_BOARD_2 & board.bishop_wb, BISHOP_WT, 1, 3)
                + worth_pos(RFC_BOARD_3 & board.bishop_wb, BISHOP_WT, 1, 2);
        return res;
    }

    int evaluate_black(Chessboard& chessboard)
    {
        auto board = chessboard.getBoard();
        int res = 0;
        auto white_p = board.get_white_pawn();
        auto white_q = board.get_white_queen();
        auto white_b = board.get_white_bishop();
        auto white_r = board.get_white_rook();
        auto white_k = board.get_white_knight();

        // Calculating queens impact
        auto pieces = board.get_black_queen();
        res += (pieces.size() - white_q.size()) * QUEEN_WT
                + worth_pos(RFC_BOARD_1 & board.queen_bb, QUEEN_WT, 1, 4)
                + worth_pos(RFC_BOARD_2 & board.queen_bb, QUEEN_WT, 1, 3)
                + worth_pos(RFC_BOARD_3 & board.queen_bb, QUEEN_WT, 1, 2);

        // Calculating rooks impact, increasing as pawns disappear
        pieces = board.get_black_rook();
        res += (pieces.size() - white_r.size())
                * (ROOK_WT - 5 * white_p.size())
                + worth_pos(BABEL_BLACK & board.rook_bb, ROOK_WT, 1, 2);

        // Calculating knights impact, decreasing as pawns disappear
        pieces = board.get_black_pawn();
        res += (board.get_black_knight().size() - white_k.size())
            * (KNIGHT_WT - 2 * (white_p.size() + pieces.size()))
                + worth_pos(RFC_BOARD_1 & board.knight_bb, KNIGHT_WT, 1, 4)
                + worth_pos(RFC_BOARD_2 & board.knight_bb, KNIGHT_WT, 1, 3)
                + worth_pos(RFC_BOARD_3 & board.knight_bb, KNIGHT_WT, 1, 2);

        // Calculating pawn impact
        res += (pieces.size() - white_p.size()) * PAWN_WT;

        // Calculating bishop impact
        pieces = board.get_black_bishop();
        res += (pieces.size() - white_b.size()) * BISHOP_WT
                + worth_pos(RFC_BOARD_1 & board.bishop_bb, BISHOP_WT, 1, 4)
                + worth_pos(RFC_BOARD_2 & board.bishop_bb, BISHOP_WT, 1, 3)
                + worth_pos(RFC_BOARD_3 & board.bishop_bb, BISHOP_WT, 1, 2);

        return res;
    }

    void find_capture(Chessboard& board, vector<Move>& moves,
                                         vector<int>& capture)
    {
        long unsigned int i = 0;
        int act = 0;
        board.getBoard().compute_danger();
        for (auto m = moves.begin(); i < moves.size(); m++)
        {
            if (m->capture_get() == PieceType::NONE)
            {
                i++;
                continue;
            }

            bit e_bit = utils::two_pow(utils::to_int(m->move_get().second));
            int temp;
            if ((board.isWhiteTurn() ? board.getBoard().white_danger & e_bit :
                              board.getBoard().black_danger & e_bit))
            {
                temp = utils::utype(m->piece_get())
                            - utils::utype(m->capture_get());
            }

            else
                temp = utils::utype(m->capture_get()) + 2;

            if (temp > act)
            {
                capture.clear();
                act = temp;
            }
            if (temp == act)
            {
                capture.push_back(i);
            }
            if (temp < 0)
            {
                moves.erase(m);
                i--;
            }
            i++;
        }
    }

    Move search(Chessboard& board, int depth)
    {
        auto moves = board.generate_legal_moves();
        if (moves.size() == 1)
            return moves[0];
        auto capture = vector<int>();
        find_capture(board, moves, capture);
        if (capture.size() != 0)
        {
            srand(time(nullptr));
            return moves[capture[(rand() % capture.size())]];
        }
        auto res = vector<int>();
        int max = INT_MIN;
        int min = INT_MAX;
        int act = rec_search(board, depth - 1, moves[0], false, max, min);
        res.push_back(0);
        for (long unsigned int i = 1; i < moves.size(); i++)
        {
            auto temp = rec_search(board, depth - 1, moves[i], false, max,
                                    min);
            auto piece = board::value_piece(moves[i].piece_get());
            auto piece_capture = board::value_piece(moves[i].capture_get());
            int worth = piece_capture - piece;
            temp += (worth > 0) ? worth * 5 : (worth + 10) * 2;


            if (temp > act)
            {
                res.clear();
                act = temp;
            }
            if (temp == act)
            {
                res.push_back(i);
            }
        }
        srand(time(nullptr));
        return moves[res[(rand() % res.size())]];
    }

    int rec_search(Chessboard& board, int depth, Move move,
                                                 bool maxmin, int max, int min)
    {
        if (depth <= 0)
        {
            auto save = board.getBoard();
            auto temp = Board(save);
            board.setBoard(temp);
            board.do_move(move);
            int res = evaluate(board);
            board.setBoard(save);
            return res;
        }
        auto save = board.getBoard();
        auto temp = Board(save);
        board.setBoard(temp);
        board.do_move(move);
        if (board.is_checkmate(board.isWhiteTurn()))
        {
            return 999999 * (maxmin ? -1 : 1);
        }
        board.setWhiteTurn(!board.isWhiteTurn());
        auto moves = board.generate_legal_moves();
        if (moves.size() == 0)
            return 9999 * (maxmin ? 1 : -1);
        int act = rec_search(board, depth - 1, moves[0], !maxmin, max, min);
        for (long unsigned int i = 1; i < moves.size(); i++)
        {
            int temp = rec_search(board, depth - 1, moves[i], !maxmin, max,
                        min) * (maxmin ? 1 : -1);
            auto piece = board::value_piece(moves[i].piece_get());
            auto piece_capture = board::value_piece(moves[i].capture_get());
            int worth = piece_capture - piece;
            temp += (worth > 0) ? worth * 5 : (worth + 10) * 2;
            act = (maxmin and act < temp) ? temp : act;
            act = (!maxmin and act > temp) ? temp : act;
            max = (maxmin and act > max) ? act : max;
            min = (!maxmin and act < min) ? act : min;

            if (max >= min)
                break;
        }
        board.setWhiteTurn(!board.isWhiteTurn());
        board.setBoard(save);
        return act;
    }
}
