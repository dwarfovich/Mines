#include "dynamic_graph_cell_item.hpp"
#include "utils.hpp"
#include "constants.hpp"

#include <QGraphicsScene>
#include <QRandomGenerator>

DynamicGraphCellItem::DynamicGraphCellItem(const Cell* cell) : GraphCellItem { cell }
{
}

void DynamicGraphCellItem::advance(int step)
{
    if (step != 1) {
        return;
    }

    using namespace constants;
    const auto   diameter     = scene()->sceneRect().width() / 2.;
    const auto   center       = mapFromScene(scene()->sceneRect().center());
    const QLineF lineToCenter = { pos(), center };
    if (lineToCenter.length() >= diameter - 200) {
        qreal angleToCenter = std::atan2(lineToCenter.dy(), lineToCenter.dx());
        angleToCenter       = normalizeAngle((pi - angleToCenter) + pi / 2);
        if (angleToCenter < pi && angleToCenter > pi / 4) {
            // Rotate left
            angle_ += (angle_ < -pi / 2) ? 0.25 : -0.25;
        } else if (angleToCenter >= pi && angleToCenter < (pi + pi / 2 + pi / 4)) {
            // Rotate right
            angle_ += (angle_ < pi / 2) ? 0.25 : -0.25;
        }
    } else if (::sin(angle_) < 0) {
        angle_ += 0.25;
    } else if (::sin(angle_) > 0) {
        angle_ -= 0.25;
    }

    speed_ += (-50 + QRandomGenerator::global()->bounded(100)) / 100.0;
    auto dx = cos(speed_) * 1.1;
    auto dy = sin(speed_) * 1.1;
    setPos(mapToParent(dx, dy));
}
