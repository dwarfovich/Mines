#include "triangle.hpp"
#include "mec.hpp"

Triangle::Triangle(const QPointF &p1, const QPointF &p2, const QPointF &p3) : points_ { p1, p2, p3 }
{
}

bool Triangle::operator==(const Triangle &rhs) const
{
    auto findPointIter = [](const auto &t, const auto &point) {
        using namespace std;
        return find(cbegin(t.points_), cend(t.points_), point);
    };

    std::array<decltype(rhs.points_.cbegin()), 3> a;
    a[0] = findPointIter(rhs, points_[0]);
    if (a[0] == rhs.points_.cend()) {
        return false;
    }
    a[1] = findPointIter(rhs, points_[1]);
    if (a[1] == rhs.points_.cend()) {
        return false;
    }
    a[2] = findPointIter(rhs, points_[2]);
    if (a[2] == rhs.points_.cend()) {
        return false;
    }

    return a[0] != a[1] && a[0] != a[2] && a[1] != a[2];

    // const auto &current_edges = edges();
    // return rhs.contains(current_edges[0]) && rhs.contains(current_edges[1]) && rhs.contains(current_edges[2]);
}

const QPointF &Triangle::operator[](int number) const
{
    return points_[number];
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
    const auto &mec = minimalEnclosingCircle(points_[0], points_[1], points_[2]);

    return mec.contains(point);
}

bool Triangle::has(const Edge &edge) const
{
    return (Edge { points_[0], points_[1] } == edge || Edge { points_[1], points_[2] } == edge
            || Edge { points_[0], points_[2] } == edge);
}

bool Triangle::has(const QPointF &point) const
{
    return (points_[0] == point || points_[1] == point || points_[2] == point);
}
