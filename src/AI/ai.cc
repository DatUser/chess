#include "ai.hh"

#define CENTER 1099494850560

namespace chess_engine {

    int evaluate_white(Chessboard& board);
    int evaluate_black(Chessboard& board);
    int rec_search(Chessboard& board, int depth, Move move,
                                                bool maxmin);
    int evaluate(Chessboard& chessboard)
    {
        if (chessboard.is_checkmate())
            return 999999;
        if (chessboard.is_draw())
            return 0;
        if (chessboard.isWhiteTurn())
            return evaluate_white(chessboard);
        return evaluate_white(chessboard);
    }

    int evaluate_white(Chessboard& chessboard)
    {
        auto board = chessboard.getBoard();
        int res = 0;
        auto black_p = board.get_black_pawn();

        // Calculating queens impact
        auto pieces = board.get_white_queen();
        res += pieces.size() * 200;

        // Calculating rooks impact, increasing as pawns disappear
        pieces = board.get_white_rook();
        res += pieces.size() * (140 - 5 * black_p.size());

        // Calculating knights impact, decreasing as pawns disappear
        pieces = board.get_white_pawn();
        res += board.get_white_knight().size()
            * (50 - 2 * (black_p.size() + pieces.size()));

        // Calculating pawn impact
        res += pieces.size() * 10;

        // Calculating bishop impact
        pieces = board.get_white_bishop();
        res += pieces.size() * 50;
        //res += chessboard.generate_legal_moves().size();
        return res;
    }

    int evaluate_black(Chessboard& chessboard)
    {
        auto board = chessboard.getBoard();
        int res = 0;
        auto white_p = board.get_white_pawn();

        // Calculating queens impact
        auto pieces = board.get_black_queen();
        res += pieces.size() * 200;

        // Calculating rooks impact, increasing as pawns disappear
        pieces = board.get_black_rook();
        res += pieces.size() * (140 - 5 * white_p.size()) ;

        // Calculating knights impact, decreasing as pawns disappear
        pieces = board.get_black_pawn();
        res += board.get_black_knight().size()
            * (50 - 2 * (white_p.size() + pieces.size()));

        // Calculating pawn impact
        res += pieces.size() * 10;

        // Calculating bishop impact
        pieces = board.get_black_bishop();
        res += pieces.size() * 50;

        //res += chessboard.generate_legal_moves().size();
        return res;
    }

    Move search(Chessboard& board, int depth)
    {
        auto moves = board.generate_legal_moves();
        auto res = vector<int>();
        int act = rec_search(board, depth - 1, moves[0], false);
        res.push_back(0);
        for (long unsigned int i = 1; i < moves.size(); i++)
        {
            auto temp = rec_search(board, depth - 1, moves[i], false);
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
        return moves[(rand() % res.size())];
    }

    int rec_search(Chessboard& board, int depth, Move move,
                                                 bool maxmin)
    {
        if (depth <= 0)
        {
            auto save = board.getBoard();
            auto temp = Board(board.getBoard());
            board.setBoard(temp);
            board.do_move(move);
            int res = evaluate(board);
            board.setBoard(save);
            return res;
        }
        auto save = board.getBoard();
        auto temp = Board(board.getBoard());
        board.setBoard(temp);
        board.do_move(move);
        board.setWhiteTurn(!board.isWhiteTurn());
        auto moves = board.generate_legal_moves();
        if (moves.size() == 0)
            return -9999;
        int act = rec_search(board, depth - 1, moves[0], !maxmin);
        for (long unsigned int i = 1; i < moves.size(); i++)
        {
            int temp = rec_search(board, depth - 1, moves[i], !maxmin);
            if (maxmin and act < temp)
                act = temp;
            else if (!maxmin and act > temp)
                act = temp;
        }
        board.setWhiteTurn(!board.isWhiteTurn());
        board.setBoard(save);
        return act;
    }
}
