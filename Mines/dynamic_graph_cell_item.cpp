#include "dynamic_graph_cell_item.hpp"
#include "utils.hpp"
#include "constants.hpp"

#include <QGraphicsScene>
#include <QRandomGenerator>

DynamicGraphCellItem::DynamicGraphCellItem(const Cell* cell) : GraphCellItem { cell }
{
    angle_ = QRandomGenerator::global()->bounded(constants::two_pi);
}

void DynamicGraphCellItem::advance(int step)
{
    if (step != 1) {
        return;
    }

    const auto   radius     = scene()->sceneRect().width() / 2.;
    const auto   center       = (scene()->sceneRect().center());
    const QLineF lineToCenter = { pos(), center };
    qreal        dx           = 0;
    qreal        dy           = 0;
    qreal        speed        = 2;
    if (lineToCenter.length() >= radius - 10) {
        angle_ = std::atan2(-lineToCenter.dy(), lineToCenter.dx());
        dx     = speed * cos(angle_);
        dy     = -speed * sin(angle_);
        if (x() <= center.x() && dx < 0) {
            dx = -dx;
        }
        if (y() <= center.y() && dy < 0) {
            dy = -dy;
        }
        //qDebug() << "OUT" << lineToCenter.length() << diameter;
        int t = 34;
    } else {
        angle_ += QRandomGenerator::global()->bounded(0.4) - 0.2;
            dx = speed * cos(angle_);
            dy = -speed * sin(angle_);
    }

    setPos(pos() + QPointF { dx, dy });
}
