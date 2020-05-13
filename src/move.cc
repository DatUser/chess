#include "all.hh"
#include "move.hh"

namespace board{

Move::Move(Position start, Position end)
: move_(std::pair<Position, Position>(start, end))
{}

}
