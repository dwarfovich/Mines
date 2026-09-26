#pragma once

#include "graph_cell_item.hpp"


class DynamicGraphCell;

class DynamicGraphCellItem : public GraphCellItem {
public:
    explicit DynamicGraphCellItem(const DynamicGraphCell* cell);

    void setAngle(qreal angle);
    void setSpeed(qreal speed);

protected:
    void advance(int step) override;

private:
    inline static int critical_radius_ = 10;

    const DynamicGraphCell* cell_ = nullptr;
};
