#include "perft_parser.hh"

namespace perft_parser {

    PerftObject parse_perft(string input)
    {
        stringstream ss(input);
        string item;
        vector<string> items;
        while (getline(ss, item, ' ')) {
            items.push_back(item);
        }
        int depth = stoi(items[items.size() - 1]);
        items.pop_back();

        auto c = Chessboard(items);
        return PerftObject(c, depth);
    }

    // ASK YAKA ABOUT CHESSBOARD CONSTRUCTOR
    Chessboard parse_fen(vector<string> splited_input)
    {
        return Chessboard(splited_input);
    }
}
