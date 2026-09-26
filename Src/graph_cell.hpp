#pragma once

#include "cell.hpp"

#include <QPoint>

class GraphCell : public Cell {
public:  // methods
    GraphCell() : Cell{} {}
    GraphCell(std::size_t new_id) : Cell{new_id} {}

    qreal x() const
    {
        return coordinates.x();
    }

    qreal y() const
    {
        return coordinates.y();
    }

public:  // data
    QPointF coordinates;
};