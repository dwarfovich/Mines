#pragma once

#include "cell.hpp"

#include <QtCore>

class GraphCell : public Cell{
public:
    GraphCell() : Cell{} {}
    GraphCell(std::size_t new_id) : Cell{new_id} {}

    qreal x = 0.;
    qreal y = 0.;
};