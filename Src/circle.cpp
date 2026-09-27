#include "circle.hpp"
#include "utils.hpp"

#include <cmath>
#include <limits>

bool Circle::contains(const QPointF& point) const
{
    static constexpr double tolerance = std::numeric_limits<double>::epsilon();

    const auto   distance = euclideanDistance(center, point);
    const double diff = distance - radius;
    if (diff < tolerance) {
        return true;
    }
    if (diff < std::fmax(std::fabs(distance), std::fabs(radius)) * tolerance) {
        return true;
    }

    return false;
}
