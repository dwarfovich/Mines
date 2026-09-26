#include "utils.hpp"

#include <numbers>
#include <string>

std::size_t QStringHasher::operator()(const QString& str) const
{
    return std::hash<std::string>()(str.toStdString());
}

qreal normalizeAngle(qreal angle)
{
    while (angle < 0) {
        angle += std::numbers::pi;
    }
    while (angle > 2. * std::numbers::pi) {
        angle -= 2. * std::numbers::pi;
    }

    return angle;
}
