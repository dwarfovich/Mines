#include "cell_item.hpp"

#include <QGraphicsSceneHoverEvent>

CellItem::CellItem(const Cell *cell)
    : cell_{cell}
{
    setAcceptHoverEvents(true);
}

const Cell *CellItem::cell() const
{
    return cell_;
}

void CellItem::setCell(Cell *cell)
{
    cell_ = cell;
}

bool CellItem::IsHovered() const
{
    return is_hovered_;
}

void CellItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    is_hovered_ = true;
    update();
    event->ignore();
}

void CellItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    is_hovered_ = false;
    update();
    event->ignore();
}