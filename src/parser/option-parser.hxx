#include <string>

#include "option-parser.hh"

inline bool OptionParser::help_get() const {
    return help_;
}

inline const std::string& OptionParser::pgn_get() const {
    return pgn_;
}

inline const std::vector<std::string>& OptionParser::listeners_get() const {
    return listeners_;
}

inline const std::string& OptionParser::perft_get() const {
    return perft_;
}
