#pragma once

#include <memory>

#include "../bitboard/all.hh"
#include "../all.hh"
#include "../utils/all.hh"

using namespace std;

namespace board {
    using shared_bit = std::shared_ptr<Bitboard>;
    class Chessboard;
    class Board {


    public:
        Board();
        Board(string ranks);
        Board(Board& board);
        Board& operator=(Board& board);

        /* Refreshes the occupied_board */
        void refresh_occupied();

        /* Compute white danger */
        void compute_danger();
        void compute_white_danger();
        void compute_black_danger();

        /* Get the state of a case at coordinate given in input */
        opt_piecetype_t is_occupied(const Position& position, Color color) const;
        bool is_occupied(shared_bit board, const Position& position) const;
        //bool is_occupied(Position position);

        /* Fait le move fait le move */
        void do_move(Move move, Color color);
        bool is_check(bool white_turn_);

        /* Get a list of position */
        vector<Position> get_white_king();
        vector<Position> get_white_queen();
        vector<Position> get_white_knight();
        vector<Position> get_white_bishop();
        vector<Position> get_white_rook();
        vector<Position> get_white_pawn();

        vector<Position> get_black_king();
        vector<Position> get_black_queen();
        vector<Position> get_black_knight();
        vector<Position> get_black_bishop();
        vector<Position> get_black_rook();
        vector<Position> get_black_pawn();

        shared_bit white_occupied_board_get();
        shared_bit black_occupied_board_get();
        shared_bit occupied_board_get();
        // DEBUG TIER
        void print();

        // Bitboards needed for the white side
        shared_bit king_wb;
        shared_bit queen_wb;
        shared_bit knight_wb;
        shared_bit bishop_wb;
        shared_bit rook_wb;
        shared_bit pawn_wb;

        // Bitboards needed for the black side
        shared_bit king_bb;
        shared_bit queen_bb;
        shared_bit knight_bb;
        shared_bit bishop_bb;
        shared_bit rook_bb;
        shared_bit pawn_bb;

        // Occupied Bitboard
        shared_bit white_occupied_board;
        shared_bit black_occupied_board;
        shared_bit occupied_board;

        // Danger cases Bitboard
        shared_bit white_danger;
        shared_bit black_danger;

    private:
        // Methods needed to compute the danger grids
        void compute_king_danger(shared_bit board, int power);
        void compute_queen_danger(shared_bit board, shared_bit ally,
                                  shared_bit enemy, shared_bit queens);
        void compute_bishop_danger(shared_bit board, shared_bit ally,
                                   shared_bit enemy, shared_bit bishops);
        void compute_rook_danger(shared_bit board, shared_bit ally,
                                 shared_bit enemy, shared_bit rooks);
        void compute_knight_danger(shared_bit board, shared_bit ally,
                                   shared_bit knights);
        void compute_pawn_danger(shared_bit board, shared_bit ally,
                                 shared_bit pawns, bool white);

        void compute_lines(shared_bit board, shared_bit ally,
                                  shared_bit enemy, int power);
        void compute_diagonals(shared_bit board, shared_bit ally,
                                  shared_bit enemy, int power);

        void put_piece(Color color, PieceType piece, Position pos);
    };
    #include "board.hxx"
}
