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

inline constexpr std::array<Direction, 4> directionsArray = {
    Direction::Up, Direction::Right, Direction::Down, Direction::Left
};

QPoint directionToShift(Direction d);
Direction nextDirection(Direction d);