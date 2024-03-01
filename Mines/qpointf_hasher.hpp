#ifndef QPOINTFHASHER_HPP
#define QPOINTFHASHER_HPP

#include <cinttypes>

class QPointF;

struct QPointFHasher {
    std::size_t operator()(const QPointF& point) const;
};

#endif // QPOINTFHASHER_HPP
