#include "triangle.hpp"

Triangle::Triangle(const QPointF &p1, const QPointF &p2, const QPointF &p3) : points_ { p1, p2, p3 }
{
}

bool Triangle::operator==(const Triangle &t) const
{
    const auto &current_edges = edges();
    return t.contains(current_edges[0]) && t.contains(current_edges[1]) && t.contains(current_edges[2]);
}

const QPointF &Triangle::operator[](int number) const
{
    return points_[number];
    /*if (number == 0)
        return p1_;
    else if (number == 1)
        return p2_;
    else
        return p3_;*/
}

std::array<Edge, 3> Triangle::edges() const
{
    return { Edge { points_[0], points_[1] }, Edge { points_[1], points_[2] }, Edge { points_[0], points_[2] } };
}

std::array<QPointF, 3> Triangle::vertices() const
{
    return points_;
}

bool Triangle::circumcircleContains(const QPointF &point) const
{
    double m12_x  = (points_[0].x() + points_[1].x()) / 2.; // x of midpoint of p1-p2 edge
    double m12_y  = (points_[0].y() + points_[1].y()) / 2.; // y of midpoint of p1-p2 edge
    double sl_12  = (points_[1].y() - points_[0].y()) / (points_[1].x() - points_[0].x()); // Slope of p1-p2 edge
    double psl_12 = -1. / sl_12;                            // Perpendicular to p1-p2 edge
    double m13_x  = (points_[0].x() + points_[2].x()) / 2.; // x of midpoint of p1-p3 edge
    double m13_y  = (points_[0].y() + points_[2].y()) / 2.; // y of midpoint of p1-p3 edge
    double sl_13  = (points_[2].y() - points_[0].y()) / (points_[2].x() - points_[0].x()); // Slope of p1-p3 edge
    double psl_13 = -1. / sl_13; // Perpendicular to p1-p3 edge
    // x of circumcenter
    double c_x = (psl_13 * m13_x + m12_y - psl_12 * m12_x - m13_y) / (psl_13 - psl_12);
    // y of circumcenter
    double c_y = psl_12 * c_x - psl_12 * m12_x + m12_y;
    // Squared radius of circumcircle
    double c_sr = (points_[0].x() - c_x) * (points_[0].x() - c_x) + (points_[0].y() - c_y) * (points_[0].y() - c_y);
    // Squared distance between circumcenter and given point
    double sd = (point.x() - c_x) * (point.x() - c_x) + (point.y() - c_y) * (point.y() - c_y);

    return sd <= c_sr;
}

bool Triangle::contains(const Edge &edge) const
{
    return (Edge { points_[0], points_[1] } == edge || Edge { points_[1], points_[2] } == edge
            || Edge { points_[0], points_[2] } == edge);
}

bool Triangle::contains(const QPointF &point) const
{
    return (points_[0] == point || points_[1] == point || points_[2] == point);
}
