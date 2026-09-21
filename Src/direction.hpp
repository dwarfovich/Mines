#pragma once

#include <QPoint>

#include <cinttypes>
#include <array>

enum class Direction : uint8_t
{
    Up,
    Right,
    Down,
    Left,
    Size
};

enum class ExtendedDirection : uint8_t
{
    Up,
    UpRight,
    Right,
    DownRight,
    Down,
    DownLeft,
    Left,
    UpLeft,
    Size
};

inline constexpr std::array<Direction, 4> directions_array = {
    Direction::Up, Direction::Right, Direction::Down, Direction::Left
};

inline constexpr std::array<ExtendedDirection, 8> extended_directions_array = {
    ExtendedDirection::Up,   ExtendedDirection::UpRight,  ExtendedDirection::Right, ExtendedDirection::DownRight,
    ExtendedDirection::Down, ExtendedDirection::DownLeft, ExtendedDirection::Left,  ExtendedDirection::UpLeft
};

QPoint    directionToShift(Direction d);
Direction nextDirection(Direction d);
QPoint    directionToShift(ExtendedDirection d);
Direction nextDirection(ExtendedDirection d);