#include "option-parser.hh"

OptionParser::OptionParser(int argc, char **argv) {
    parse_option(argc, argv);
}

void OptionParser::unknown_arg() {
    help_ = false;
    pgn_ = "";
    listeners_.clear();
    perft_ = "";
}

void OptionParser::parse_option(int argc, char **argv) {
    std::string tmp;
    for (int i = 1; i < argc; i++) {
        tmp = argv[i];
        if (tmp == "-h" or tmp == "--help")
            help_ = true;

        else if (tmp == "--pgn") {
            pgn_ = (i + 1 < argc) ? argv[i + 1] : "";
            i++;
        }

        else if (tmp =="-l" or tmp == "--listeners") {
            while (i < argc) {
                listeners_.push_back(tmp);
                i++;
            }
        }

        else if (tmp == "--perft") {
            perft_ = (i + 1 < argc) ? argv[i + 1] : "";
            i++;
        }

        else {
            unknown_arg();
            break;
        }
    }
}
