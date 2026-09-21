#pragma once

#include <cstddef>

class QPoint;

struct QPointHasher
{
    std::size_t operator()(const QPoint& point) const;
};
