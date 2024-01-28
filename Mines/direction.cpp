#include "direction.hpp"

QPoint directionToShift(Direction d)
{
    switch (d) {
        case Direction::Up: return { 0, -1 }; break;
        case Direction::Right: return { 1, 0 }; break;
        case Direction::Down: return { 0, 1 }; break;
        case Direction::Left: return { -1, 0 }; break;
        default: return {};
    }
}

Direction nextDirection(Direction d)
{
    auto n = static_cast<std::underlying_type_t<Direction>>(d);
    (n >= 3 ? n = 0 : ++n);
   
    return Direction(n);
}
