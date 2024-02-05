#pragma once

class QPoint;

struct QPointHasher
{
    size_t operator()(const QPoint& point) const;
};
