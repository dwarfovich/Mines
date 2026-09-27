#pragma once

#include "circle.hpp"

#include <vector>

bool    isEnclosingCircle(const Circle& circle, const auto& points);
QPointF circleCenter(double x1, double y1, double x2, double y2);
Circle  minimalEnclosingCircle(const QPointF& p1, const QPointF& p2, const QPointF& p3);
Circle  minimalEnclosingCircle(const QPointF& p1, const QPointF& p2);
Circle  trivialMinimalCircle(const std::vector<QPointF>& points);
template <typename CellType>
Circle minimalEnclosingCircleWelzl(const std::vector<CellType>& cells,
                                   std::vector<QPointF>         restPoints,
                                   size_t                       firstIndex)
{
    if (firstIndex == cells.size() || restPoints.size() == 3) {
        return trivialMinimalCircle(restPoints);
    }

    auto circle = minimalEnclosingCircleWelzl(cells, restPoints, firstIndex + 1);
    if (circle.contains(cells[firstIndex].coordinates)) {
        return circle;
    }

    restPoints.push_back(cells[firstIndex].coordinates);

    return minimalEnclosingCircleWelzl(cells, restPoints, firstIndex + 1);
}

// Uses Welzl' algorithm.
template <typename CellType>
Circle minimalEnclosingCircle(const std::vector<CellType>& cells)
{
    return minimalEnclosingCircleWelzl(cells, {}, 0);
}
