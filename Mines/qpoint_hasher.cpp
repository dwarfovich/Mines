#include "qpoint_hasher.hpp"

#include <QPoint>

size_t QPointHasher::operator()(const QPoint& point) const
{
    return std::hash<int>()(point.x()) * 31 ^ std::hash<int>()(point.y()) * 53;
}
