#include "move.hh"
namespace board {

inline std::pair<Position, Position> Move::move_get() const {
    return move_;
}
    
inline void Move::piece_set(PieceType piece) {
    piece_ = piece;
}

inline PieceType Move::piece_get() {
    return piece_;
}

inline void Move::promotion_set(PieceType promotion) {
    promotion_ = promotion;
}

inline PieceType Move::promotion_get() {
    return promotion_;
}

inline void Move::capture_set(PieceType capture) {
    capture_ = capture;
}

inline PieceType Move::capture_get() {
    return capture_;
}

inline void Move::double_pawn_push_set(bool double_pawn_push) {
    double_pawn_push_ = double_pawn_push;
}

inline bool Move::double_pawn_push_get() {
    return double_pawn_push_;
}

inline void Move::king_castling_set(bool king_castling) {
    king_castling_ = king_castling;
}

inline bool Move::king_castling_get() {
    return king_castling_;
}

inline void Move::queen_castling_set(bool queen_castling) {
    queen_castling_ = queen_castling;
}

inline bool Move::queen_castling_get() {
    return queen_castling_;
}

inline void Move::en_passant_set(bool en_passant) {
    en_passant_ = en_passant;
}

inline bool Move::en_passant_get() {
    return en_passant_;
}

}
