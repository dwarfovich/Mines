#pragma once

#include "graph_cell_item.hpp"

#include "dynamic_graph_cell.hpp"
class DynamicGraphCell;

class DynamicGraphCellItem : public GraphCellItem {
public:
    explicit DynamicGraphCellItem(const DynamicGraphCell* cell);

    void setAngle(qreal angle);
    void setSpeed(qreal speed);

    std::size_t cellId() const override{
        return cell_->id;
    }

protected:
    void advance(int step) override;

private:
    inline static int critical_radius_ = 10;

    const DynamicGraphCell* cell_ = nullptr;
};
