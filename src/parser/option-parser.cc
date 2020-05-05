OptionParser::OptionParser(int argc, char **argv) {
    parse_option(argc, argv);
}

void OptionParser::unknown_arg() {
    help_ = false;
    png_ = "";
    listeners_.clear();
    perft_ = "";
}

void OptionParser::parse_option(int argc, char **argv) {
    for (int i = 0; i < argc; i++) {
        switch (argv[i]) {
        case "-h":
        case "--help":
            help_ = true;
            break;

        case "--pgn":
            pgn_ = (i + 1 < argc) ? argv[i + 1] : "";
            break;

        case "-l":
        case "--listeners":

        }
    }
}
