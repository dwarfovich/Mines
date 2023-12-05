#include "mec.hpp"
#include "utils.hpp"

bool isEnclosingCircle(const Circle& circle, const auto& points)
{
    for (const auto& point : points) {
        if (!circle.contains(point)) {
            return false;
        }
    }

    return true;
}

QPointF circleCenter(double x1, double y1, double x2, double y2)
{
    double a = x1 * x1 + y1 * y1;
    double b = x2 * x2 + y2 * y2;
    double c = x1 * y2 - y1 * x2;

    return { (y2 * a - y1 * b) / (2. * c), (x1 * b - x2 * a) / (2. * c) };
}

Circle minimalEnclosingCircle(const QPointF& p1, const QPointF& p2, const QPointF& p3)
{
    QPointF center = circleCenter(p2.x() - p1.x(), p2.y() - p1.y(), p3.x() - p1.x(), p3.y() - p1.y());
    center.setX(center.x() + p1.x());
    center.setY(center.y() + p1.y());

    return { center, euclideanDistance(center, p1) };
}

Circle minimalEnclosingCircle(const QPointF& p1, const QPointF& p2)
{
    QPointF center = { (p1.x() + p2.x()) / 2., (p1.y() + p2.y()) / 2. };

    return { center, euclideanDistance(p1, p2) / 2.0 };
}

Circle trivialMinimalCircle(const std::vector<QPointF>& points)
{
    Q_ASSERT(points.size() <= 3);

    if (points.empty()) {
        return { { 0, 0 }, 0 };
    } else if (points.size() == 1) {
        return { points.front(), 0 };
    } else if (points.size() == 2) {
        return minimalEnclosingCircle(points.front(), points.back());
    }

    for (int i = 0; i < points.size(); ++i) {
        for (int j = i + 1; j < points.size(); ++j) {
            Circle circle = minimalEnclosingCircle(points[i], points[j]);
            if (isEnclosingCircle(circle, points)) {
                return circle;
            }
        }
    }

    return minimalEnclosingCircle(points.front(), points[1], points.back());
}

Circle minimalEnclosingCircleWelzl(const std::vector<QPointF>& points,
                                   std::vector<QPointF>        restPoints,
                                   size_t                      firstIndex)
{
    if (firstIndex == points.size() || restPoints.size() == 3) {
        return trivialMinimalCircle(restPoints);
    }

    auto circle = minimalEnclosingCircleWelzl(points, restPoints, firstIndex + 1);
    if (circle.contains(points[firstIndex])) {
        return circle;
    }

    restPoints.push_back(points[firstIndex]);

    return minimalEnclosingCircleWelzl(points, restPoints, firstIndex + 1);
}

Circle minimalEnclosingCircle(const std::vector<QPointF>& points)
{
    return minimalEnclosingCircleWelzl(points, {}, 0);
}
