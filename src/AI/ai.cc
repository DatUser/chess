#include "ai.hh"
#include <climits>

#define CENTER 1099494850560
#define QUEEN_WT 1000
#define ROOK_WT 200
#define KNIGHT_WT 150
#define BISHOP_WT 200
#define PAWN_WT 10

namespace chess_engine {

    int evaluate_white(Chessboard& board);
    int evaluate_black(Chessboard& board);
    int rec_search(Chessboard& board, int depth, Move move,
                                                bool maxmin, int max, int min);
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
        res += (pieces.size() - black_q.size()) * QUEEN_WT;

        // Calculating rooks impact, increasing as pawns disappear
        pieces = board.get_white_rook();
        res += (pieces.size() - black_r.size())
                * (ROOK_WT - 5 * black_p.size());

        // Calculating knights impact, decreasing as pawns disappear
        pieces = board.get_white_pawn();
        res += (board.get_white_knight().size() - black_k.size())
            * (KNIGHT_WT - 2 * (black_p.size() + pieces.size()));

        // Calculating pawn impact
        res += (pieces.size() - black_p.size()) * PAWN_WT;

        // Calculating bishop impact
        pieces = board.get_white_bishop();
        res += (pieces.size() - black_b.size()) * BISHOP_WT;
        //res += chessboard.generate_legal_moves().size();
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
        res += (pieces.size() - white_q.size()) * QUEEN_WT;

        // Calculating rooks impact, increasing as pawns disappear
        pieces = board.get_black_rook();
        res += (pieces.size() - white_r.size())
                * (ROOK_WT - 5 * white_p.size());

        // Calculating knights impact, decreasing as pawns disappear
        pieces = board.get_black_pawn();
        res += (board.get_black_knight().size() - white_k.size())
            * (KNIGHT_WT - 2 * (white_p.size() + pieces.size()));

        // Calculating pawn impact
        res += (pieces.size() - white_p.size()) * PAWN_WT;

        // Calculating bishop impact
        pieces = board.get_black_bishop();
        res += (pieces.size() - white_b.size()) * BISHOP_WT;

        //res += chessboard.generate_legal_moves().size();
        return res;
    }

    Move search(Chessboard& board, int depth)
    {
        auto moves = board.generate_legal_moves();
        auto res = vector<int>();
        int max = INT_MIN;
        int min = INT_MAX;
        int act = rec_search(board, depth - 1, moves[0], false, max, min);
        res.push_back(0);
        for (long unsigned int i = 1; i < moves.size(); i++)
        {
            auto temp = rec_search(board, depth - 1, moves[i], false, max,
                                    min);
            auto piece_capture = moves[i].capture_get();
            if (piece_capture != PieceType::NONE)
            {
                switch (piece_capture)
                {
                    case PieceType::QUEEN:
                        temp += 500;
                        break;
                    case PieceType::ROOK:
                        temp += 150;
                        break;
                    case PieceType::BISHOP:
                    case PieceType::KNIGHT:
                        temp += 200;
                        break;
                    default:
                        temp += 10;
                        break;
                }
            }
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
        //std::cout << "size: " << res.size() << "\n";
        //std::cout << "rand: " << rand() % res.size() << "\n";
        srand(time(NULL));
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
            auto piece_capture = moves[i].capture_get();
            if (piece_capture != PieceType::NONE)
            {
                switch (piece_capture)
                {
                    case PieceType::QUEEN:
                        temp += 100 * (maxmin ? 1 : -1);
                        break;
                    case PieceType::ROOK:
                        temp += 50 * (maxmin ? 1 : -1);
                        break;
                    case PieceType::BISHOP:
                    case PieceType::KNIGHT:
                        temp += 30 * (maxmin ? 1 : -1);
                        break;
                    default:
                        temp += 10 * (maxmin ? 1 : -1);
                        break;
                }
            }
            /*if (maxmin and act < temp)
                act = temp;
            else if (!maxmin and act > temp)
                act = temp;*/
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
