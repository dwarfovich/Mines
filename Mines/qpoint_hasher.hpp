#pragma once

#include <cinttypes>

class QPoint;

struct QPointHasher
{
    std::size_t operator()(const QPoint& point) const;
};
