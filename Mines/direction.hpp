#pragma once

#include <QPoint>

#include <cinttypes>

enum class Direction : uint8_t
{
    Up,
    Right,
    Down,
    Left,
    Size
};

QPoint directionToShift(Direction d);
Direction nextDirection(Direction d);