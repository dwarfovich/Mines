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

QPoint directionToShift(ExtendedDirection d)
{
    switch (d) {
        case ExtendedDirection::Up: return { 0, -1 };
        case ExtendedDirection::UpRight: return {1,-1};
        case ExtendedDirection::Right: return {1, 0};
        case ExtendedDirection::DownRight: return {1, 1};
        case ExtendedDirection::Down: return {0, 1};
        case ExtendedDirection::DownLeft: return {-1, 1};
        case ExtendedDirection::Left: return {-1, 0};
        case ExtendedDirection::UpLeft: return {-1, -1};
        default: return {};
    }
}

Direction nextDirection(ExtendedDirection d)
{
    auto n = static_cast<std::underlying_type_t<ExtendedDirection>>(d);
    (n >= 7 ? n = 0 : ++n);

    return Direction(n);
}
