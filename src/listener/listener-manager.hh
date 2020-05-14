#include <vector>
#include <chessboard/chessboard.hh>

namespace listener {

class ListenerManager {

    public:
        ListenerManager& instance();

        void listeners_set(std::vector<Listener*>& listeners);
        const std::vector<Listener*>& listeners_get() const;

        void chessboard_set(board::Chessboard& chessboard);
        const board::Chessboard chessboard_get() const;

    private:
        ListenerManager() = default;
        ~ListenerManager() = default;
        std::vector<Listener*> listeners_;
        board::Chessboard chessboard_;
};

#include "listener-manager.hxx"

}
