#include "dynamic_graph_cell_item.hpp"
#include "dynamic_graph_cell.hpp"
#include "graph_boards_constants.hpp"
#include "utils.hpp"

#include <QGraphicsScene>
#include <QRandomGenerator>

DynamicGraphCellItem::DynamicGraphCellItem(const DynamicGraphCell* cell) : GraphCellItem{cell}, cell_{cell}
{
    // angle_ = QRandomGenerator::global()->bounded(constants::two_pi);
}

void DynamicGraphCellItem::setAngle(qreal angle)
{
    // angle_ = angle;
}

void DynamicGraphCellItem::setSpeed(qreal speed)
{
    // speed_ = speed;
}

void DynamicGraphCellItem::advance(int step)
{
    if (step == 0) {
        setPos(QPointF{cell_->x(), cell_->y()});
    }
}
