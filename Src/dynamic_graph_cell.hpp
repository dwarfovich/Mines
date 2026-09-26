#pragma once

#include "graph_cell.hpp"

class DynamicGraphCell : public GraphCell {
public:
    DynamicGraphCell() : GraphCell{} {};
    DynamicGraphCell(std::size_t id) : GraphCell{id} {};

    qreal angle = 30.;
    qreal speed = 1;
};
