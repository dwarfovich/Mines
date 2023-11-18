#include "edge.hpp"

Edge::Edge(QPointF start_point, QPointF end_point)
    : point1{start_point}
    , point2{end_point}
{}

bool Edge::operator==(const Edge &edge) const
{
    return (point1 == edge.point1 && point2 == edge.point2)
            || (point1 == edge.point2 && point2 == edge.point1);
}
