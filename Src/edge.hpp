#pragma once

#include <QPointF>

class Edge {
public:
    Edge(QPointF start_point = QPointF{}, QPointF end_point = QPointF{});

    QPointF&       operator[](int index);
    const QPointF& operator[](int index) const;
    bool           operator==(const Edge& edge) const;

    const std::pair<QPointF, QPointF>& points() const;

private:
    std::pair<QPointF, QPointF> points_;
};

struct EdgeHasher {
    std::size_t operator()(const Edge& e) const;
};
