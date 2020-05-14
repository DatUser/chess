#include <iostream>
#include <fstream>

#include "parser/option-parser.hh"
#include "perft_parser/perft_parser.hh"

int main(int argc, char** argv) {
    OptionParser parser(argc, argv);

    if (parser.help_get()) {
        std::cout << "-h [ --help ]             show usage" << std::endl
                  << "--pgn                     arg path to the PGN game file" << std::endl
                  << "-l [ --listeners ] args   list of paths to listener plugins" << std::endl
                  << "--perft arg               path to a perft file" << std::endl;
    }

    if (parser.perft_get().compare(""))
    {
        ifstream file;
        file.open(parser.perft_get());
        if (!file.is_open())
            return 1;
        string line;
        getline(file, line);
        auto pobject = perft_parser::parse_perft(line);
        auto moves = pobject.chessboard_get().generate_legal_moves();
        for (long unsigned int i = 0; i < moves.size(); i++) {
            auto move = moves[i];
            auto brank = utils::utype(move.move_get().first.rank_get());
            auto bfile = utils::utype(move.move_get().first.file_get());
            auto erank = utils::utype(move.move_get().second.rank_get());
            auto efile = utils::utype(move.move_get().second.file_get());
            std::cout << brank << "/" << bfile << " " << erank << "/" << efile << "\n";
        }
        pobject.chessboard_get().print();
        std::cout << moves.size() << std::endl;
    }

    return 0;
}
