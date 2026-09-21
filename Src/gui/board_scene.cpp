#include "board_scene.hpp"
#include "cell_item.hpp"
#include "sprite_cell_item.hpp"

#include <QGraphicsSceneMouseEvent>

BoardScene::BoardScene(QObject *parent) : QGraphicsScene { parent }
{
    connect(&timer_, &QTimer::timeout, this, &BoardScene::advance);
}

void BoardScene::registerCellItem(CellItem *cell_item)
{
    cell_items_.insert({ cell_item->cell(), cell_item });
    addItem(cell_item);
}

void BoardScene::updateCellItemForCell(Cell *cell)
{
    auto iter = cell_items_.find(cell);
    if (iter != cell_items_.cend()) {
        iter->second->update();
    }
}

void BoardScene::clear()
{
    cell_items_.clear();
    QGraphicsScene::clear();
}

void BoardScene::startAnimation()
{
    if (advance_period_) {
        timer_.start(advance_period_);
    }
}

void BoardScene::stopAnimation()
{
    timer_.stop();
}

void BoardScene::setNotAnimated()
{
    timer_.stop();
    advance_period_ = 0;
}

void BoardScene::setAdvancePeriod(int period)
{
    advance_period_ = period;
}

void BoardScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    auto cell_item = dynamic_cast<CellItem *>(itemAt(event->scenePos(), QTransform()));
    if (cell_item) {
        emit cellItemClicked(cell_item, event);
    }
}
