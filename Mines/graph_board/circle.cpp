#include "Circle.hpp"
#include "utils.hpp"

#include <cmath>

bool Circle::contains(const QPointF& point) const
{
    return euclideanDistance(center, point) <= radius;
}
