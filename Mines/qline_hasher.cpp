#include "qline_hasher.hpp"
#include "qpoint_hasher.hpp"

#include <QLine>

size_t QLineHasher::operator()(const QLine& line) const
{
    return QPointHasher()(line.p1()) * 31 ^ QPointHasher()(line.p2()) * 53;
}
