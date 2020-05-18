#include "all.hh"
#include "move.hh"

namespace board
{
    Move::Move(Position start, Position end)
    : move_(std::pair<Position, Position>(start, end))
    {}

    bool Move::operator==(const Move& move)
    {
        std::pair<Position, Position> pos1 = move_get();
        std::pair<Position, Position> pos2 = move.move_get();
        if (pos1.first == pos2.first and pos1.second == pos2.second)
        {
            return true;
        }

        return false;
    }
}
