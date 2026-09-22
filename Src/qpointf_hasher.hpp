#pragma once

#include <cstddef>

class QPointF;

struct QPointFHasher {
    std::size_t operator()(const QPointF& point) const;
};
