#pragma once

#include <vector>
#include <string>

class OptionParser {
    public:

        OptionParser(int argc, char **argv);

        ~OptionParser() = default;

        bool help_get() const;

        const std::string& pgn_get() const;

        const std::vector<std::string>& listeners_get() const;

        const std::string& perft_get() const;

    private:
        void parse_option(int argc, char **argv);
        void unknown_arg();

        bool help_ = false;
        std::string pgn_ = "";
        std::vector<std::string> listeners_;
        std::string perft_ = "";
};

#include "option-parser.hxx"
