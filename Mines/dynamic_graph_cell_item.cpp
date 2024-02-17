#include "dynamic_graph_cell_item.hpp"
#include "utils.hpp"
#include "constants.hpp"
#include "graph_boards_constants.hpp"

#include <QGraphicsScene>
#include <QRandomGenerator>

DynamicGraphCellItem::DynamicGraphCellItem(const Cell* cell) : GraphCellItem { cell }
{
    angle_ = QRandomGenerator::global()->bounded(constants::two_pi);
}

void DynamicGraphCellItem::setAngle(qreal angle)
{
    angle_ = angle;
}

void DynamicGraphCellItem::setSpeed(qreal speed)
{
    speed_ = speed;
}

void DynamicGraphCellItem::advance(int step)
{
    if (step != 1) {
        return;
    }

    const auto   radius       = scene()->sceneRect().width() / 2.;
    const auto   center       = (scene()->sceneRect().center());
    const QLineF lineToCenter = { pos(), center };
    qreal        dx           = 0;
    qreal        dy           = 0;
    if (lineToCenter.length() >= radius - 10) {
        angle_ = std::atan2(-lineToCenter.dy(), lineToCenter.dx());
        dx     = speed_ * cos(angle_);
        dy     = -speed_ * sin(angle_);
    } else {
        angle_ += QRandomGenerator::global()->bounded(constants::graph_board::random_angle_range)
                  - constants::graph_board::random_angle_range / 2.;
        dx = speed_ * cos(angle_);
        dy = -speed_ * sin(angle_);
    }

    setPos(pos() + QPointF { dx, dy });
}
