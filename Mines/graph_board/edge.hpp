#ifndef EDGE_HPP
#define EDGE_HPP

#include <QPointF>

class Edge
{
public:
    Edge(QPointF start_point = QPointF{}, QPointF end_point = QPointF{});
    bool operator== (const Edge& edge) const;

    QPointF point1;
    QPointF point2;
};

#endif // EDGE_HPP
