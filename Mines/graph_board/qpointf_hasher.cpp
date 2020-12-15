#include "qpointf_hasher.hpp"

#include <QPointF>

size_t QPointFHasher::operator()(const QPointF point) const
{
    return std::hash<qreal>()(point.x()) * 31 ^ std::hash<qreal>()(point.y()) * 53;
}
