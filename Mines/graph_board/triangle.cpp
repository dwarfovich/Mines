#include "triangle.hpp"

Triangle::Triangle(const QPointF& p1, const QPointF& p2, const QPointF& p3)
    : p1_{p1}
    , p2_{p2}
    , p3_{p3}
{}

bool Triangle::operator== (const Triangle &t) const
{
    const auto& current_edges = edges();
    return t.contains(current_edges[0]) && t.contains(current_edges[1]) && t.contains(current_edges[2]);
}

const QPointF &Triangle::operator[](int number) const
{
    if (number == 0) return p1_;
    else if (number == 1) return p2_;
    else return p3_;
}

std::array<Edge, 3> Triangle::edges() const
{
    return { Edge{p1_, p2_}, Edge{p2_, p3_}, Edge{p1_, p3_} };
}

std::array<QPointF, 3> Triangle::vertices() const
{
    return {p1_, p2_, p3_};
}

bool Triangle::circumcircleContains(const QPointF &point) const
{
    double m12_x = (p1_.x() + p2_.x()) / 2.;                  // x of midpoint of p1-p2 edge
    double m12_y = (p1_.y() + p2_.y()) / 2.;                  // y of midpoint of p1-p2 edge
    double sl_12 = (p2_.y() - p1_.y()) / (p2_.x() - p1_.x()); // Slope of p1-p2 edge
    double psl_12 = -1. / sl_12;                              // Perpendicular to p1-p2 edge
    double m13_x = (p1_.x() + p3_.x()) / 2.;                  // x of midpoint of p1-p3 edge
    double m13_y = (p1_.y() + p3_.y()) / 2.;                  // y of midpoint of p1-p3 edge
    double sl_13 = (p3_.y() - p1_.y()) / (p3_.x() - p1_.x()); // Slope of p1-p3 edge
    double psl_13 = -1. / sl_13;                              // Perpendicular to p1-p3 edge
    // x of circumcenter
    double c_x = (psl_13 * m13_x + m12_y - psl_12 * m12_x - m13_y) / (psl_13 - psl_12);
    // y of circumcenter
    double c_y = psl_12 * c_x - psl_12 * m12_x + m12_y;
    // Squared radius of circumcircle
    double c_sr = (p1_.x() - c_x) * (p1_.x() - c_x) + (p1_.y() - c_y) * (p1_.y() - c_y);
    // Squared distance between circumcenter and given point
    double sd = (point.x() - c_x) * (point.x() - c_x) + (point.y() - c_y) * (point.y() - c_y);

    return sd <= c_sr;
}

bool Triangle::contains(const Edge &edge) const
{
    return (   Edge{p1_, p2_} == edge
            || Edge{p2_, p3_} == edge
            || Edge{p1_, p3_} == edge);
}

bool Triangle::contains (const QPointF &point) const
{
    return (p1_ == point || p2_ == point || p3_ == point);
}
