#include "perft_parser.hh"
#include <listener/listener-manager.hh>
#include "listener.hh"

namespace perft_parser {

    using ListenerManager = listener::ListenerManager;

    PerftObject parse_perft(string input)
    {
        stringstream ss(input);
        string item;
        vector<string> items;
        while (getline(ss, item, ' ')) {
            items.push_back(item);
        }
        if (items[0].compare("position") == 0) {
            items.erase(items.begin());
        }
        if (items[0].compare("fen") == 0) {
            items.erase(items.begin());
        }
        int depth = stoi(items[items.size() - 1]);
        items.pop_back();

        auto c = Chessboard(items);
        ListenerManager::instance().chessboard_set(&c);
        auto listeners = ListenerManager::instance().listeners_get();
        for (auto listener : listeners)
            listener->register_board(c);
        return PerftObject(c, depth);
    }

    // ASK YAKA ABOUT CHESSBOARD CONSTRUCTOR
    Chessboard parse_fen(vector<string> splited_input)
    {
        auto c = Chessboard(splited_input);
        ListenerManager::instance().chessboard_set(&c);
        auto listeners = ListenerManager::instance().listeners_get();
        for (auto listener : listeners)
            listener->register_board(c);

        return c;
    }
}
