#include "board_scene.hpp"
#include "cell_item.hpp"
#include "standard_cell_item.hpp"

#include <QGraphicsSceneMouseEvent>

BoardScene::BoardScene(QObject* parent)
    : QGraphicsScene{parent}
{}

void BoardScene::registerCellItem(CellItem *cell_item)
{
    cell_items_.insert({cell_item->cell(), cell_item});
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

void BoardScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    auto cell_item = dynamic_cast<CellItem*>(itemAt(event->scenePos(), QTransform()));
    if (cell_item) {
        emit cellItemClicked(cell_item, event);
    }
}
