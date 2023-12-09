#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "edge.hpp"

#include <QPointF>

#include <array>

class Triangle
{
public:
    Triangle(const QPointF& p1 = QPointF {}, const QPointF& p2 = QPointF {}, const QPointF& p3 = QPointF {});
    bool           operator==(const Triangle& t) const;
    const QPointF& operator[](int number) const;

    std::array<Edge, 3>    edges() const;
    std::array<QPointF, 3> vertices() const;
    bool                   circumcircleContains(const QPointF& point) const;
    bool                   has(const Edge& edge) const;
    bool                   has(const QPointF& point) const;

private:
    std::array<QPointF, 3> points_;
};

#endif // TRIANGLE_HPP
