#pragma once

#include "graph_cell_item.hpp"

class Cell;

class DynamicGraphCellItem : public GraphCellItem
{
public:
    explicit DynamicGraphCellItem(const Cell *cell);

protected:
    void advance(int step) override;

private:
    qreal max_speed_ = 200.;
    qreal speed_ = 50.;
    qreal angle_     = 0.;
};
