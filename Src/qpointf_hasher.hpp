#ifndef QPOINTFHASHER_HPP
#define QPOINTFHASHER_HPP

#include <cstddef>

class QPointF;

struct QPointFHasher {
    std::size_t operator()(const QPointF& point) const;
};

#endif // QPOINTFHASHER_HPP
