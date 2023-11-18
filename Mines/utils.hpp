#ifndef UTILS_HPP
#define UTILS_HPP

#include <QString>

struct QStringHasher {
    size_t operator() (const QString& str) const;
};

#endif // UTILS_HPP
