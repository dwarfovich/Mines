#ifndef MEC_HPP
#define MEC_HPP

#include "circle.hpp"

#include <vector>

bool    isEnclosingCircle(const Circle& circle, const auto& points);
QPointF circleCenter(double x1, double y1, double x2, double y2);
Circle  minimalEnclosingCircle(const QPointF& p1, const QPointF& p2, const QPointF& p3);
Circle  minimalEnclosingCircle(const QPointF& p1, const QPointF& p2);
Circle  trivialMinimalCircle(const std::vector<QPointF>& points);
Circle  minimalEnclosingCircleWelzl(const std::vector<QPointF>& points, std::vector<QPointF> R, size_t firstIndex);
// Uses Welzl' algorithm.
Circle minimalEnclosingCircle(const std::vector<QPointF>& points);

#endif