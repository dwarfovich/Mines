#include "utils.hpp"
#include "constants.hpp"

#include <string>

size_t QStringHasher::operator()(const QString &str) const {
    return std::hash<std::string>()(str.toStdString());
}

qreal normalizeAngle(qreal angle)
{
    while (angle < 0) {
        angle += constants::two_pi;
    }
    while (angle > constants::two_pi) {
        angle -= constants::two_pi;
    }

    return angle;
}
