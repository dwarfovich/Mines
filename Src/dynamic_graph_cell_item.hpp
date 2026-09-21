#pragma once

#include "graph_cell_item.hpp"

class Cell;

class DynamicGraphCellItem : public GraphCellItem
{
public:
    explicit DynamicGraphCellItem(const Cell *cell);

    void setAngle(qreal angle);
    void setSpeed(qreal speed);

protected:
    void advance(int step) override;

private:
    inline static int critical_radius_ = 10;
    qreal speed_ = 2.;
    qreal angle_     = 0.;
};
