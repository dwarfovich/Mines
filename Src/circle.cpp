#include "circle.hpp"
#include "utils.hpp"

#include <limits>
#include <cmath>

bool Circle::contains(const QPointF& point) const
{
    static constexpr double tolerance = std::numeric_limits<double>::epsilon();
    auto                    distance  = euclideanDistance(center, point);
    double                  diff      = distance - radius;
    if (diff < tolerance) {
        return true;
    }

    if (diff < std::fmax(std::fabs(distance), std::fabs(radius)) * tolerance) {
        return true;
    }

    return false;
}
