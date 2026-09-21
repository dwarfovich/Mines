#include "edge.hpp"
#include "std_pair_hasher.hpp"
#include "qpointf_hasher.hpp"

Edge::Edge(QPointF start_point, QPointF end_point) : points_ { start_point, end_point }
{
}

QPointF &Edge::operator[](int index)
{
    return (index == 0 ? points_.first : points_.second);
}

const QPointF &Edge::operator[](int index) const
{
    return (index == 0 ? points_.first : points_.second);
}

bool Edge::operator==(const Edge &edge) const
{
    return ((*this)[0] == edge[0] && (*this)[1] == edge[1]) || ((*this)[0] == edge[1] && (*this)[1] == edge[0]);
}

const std::pair<QPointF, QPointF> &Edge::points() const
{
    return points_;
}

std::size_t EdgeHasher::operator()(const Edge &e) const
{
    return StdPairHasher<QPointF, QPointF, QPointFHasher, QPointFHasher>()(e.points());
}
