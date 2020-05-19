namespace board
{
    inline Position::Position(File file, Rank rank)
        : file_(file)
        , rank_(rank)
    {}

    inline bool Position::operator==(const Position& pos) const
    {
        return file_get() == pos.file_get() && rank_get() == pos.rank_get();
    }

    inline bool Position::operator!=(const Position& pos) const
    {
        return !(*this == pos);
    }

    inline File Position::file_get() const
    {
        return file_;
    }

    inline Rank Position::rank_get() const
    {
        return rank_;
    }

    inline std::string pos_to_string(const Position& p)
    {
        std::string out = "";

        switch (p.file_get())
        {
        case File::A:
            out += "a";
            break;
        case File::B:
            out += "b";
            break;
        case File::C:
            out += "c";
            break;
        case File::D:
            out += "d";
            break;
        case File::E:
            out += "e";
            break;
        case File::F:
            out += "f";
            break;
        case File::G:
            out += "g";
            break;
        case File::H:
            out += "h";
            break;
        default:
            throw std::invalid_argument("Unknown piece");
        }

        switch (p.rank_get())
        {
        case Rank::ONE:
            out += "1";
            break;
        case Rank::TWO:
            out += "2";
            break;
        case Rank::THREE:
            out += "3";
            break;
        case Rank::FOUR:
            out += "4";
            break;
        case Rank::FIVE:
            out += "5";
            break;
        case Rank::SIX:
            out += "6";
            break;
        case Rank::SEVEN:
            out += "7";
            break;
        case Rank::EIGHT:
            out += "8";
            break;
        default:
            throw std::invalid_argument("Unknown piece");
        }

        return out;
    }

} // namespace board
