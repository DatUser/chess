inline void ListenerManager::listeners_set(std::vector<Listener*>& listeners) {
    listeners_ = listeners;
}

inline const std::vector<Listener*>& ListenerManager::listeners_get() const {
    return listeners_;
}

inline void ListenerManager::chessboard_set(board::Chessboard& chessboard) {
    chessboard_ = chessboard;
}

inline const board::Chessboard ListenerManager::chessboard_get() const {
    return chessboard_;
}
