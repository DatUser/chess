#include <iostream>

#include <parser/option-parser.hh>

int main(int argc, char** argv) {
    OptionParser parser(argc, argv);

    if (parser.help_get()) {
        std::cout << "-h [ --help ]             show usage" << std::endl
                  << "--pgn                     arg path to the PGN game file" << std::endl
                  << "-l [ --listeners ] args   list of paths to listener plugins" << std::endl
                  << "--perft arg               path to a perft file" << std::endl;
    }

    return 0;
}
