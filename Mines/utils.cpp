#include "utils.hpp"

#include <string>

size_t QStringHasher::operator()(const QString &str) const {
    return std::hash<std::string>()(str.toStdString());
}

double euclideanDistance(const QPointF &p1, const QPointF &p2)
{
    return std::sqrt(sqr(p1.x() - p2.x()) + sqr(p1.y() - p2.y()));
}
