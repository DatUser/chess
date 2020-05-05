class OptionParser {
    public:

        OptionParser(int argc, char **argv);

        ~OptionParser() = default;

    private:
        void parse_option(int argc, char **argv);
        void unknown_arg();

        bool help_ = false;
        std::string pgn_ = "";
        std::vector<std::string> listeners_;
        std::string perft_ = "";
};
