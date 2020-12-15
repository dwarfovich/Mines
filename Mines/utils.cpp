#include "utils.hpp"

#include <string>

size_t QStringHasher::operator()(const QString &str) const {
    return std::hash<std::string>()(str.toStdString());
}
