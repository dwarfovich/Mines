#ifndef QPOINTFHASHER_HPP
#define QPOINTFHASHER_HPP

class QPointF;

struct QPointFHasher {
    size_t operator()(const QPointF& point) const;
};

#endif // QPOINTFHASHER_HPP
