#include "cell_item.hpp"
#include "cell.hpp"

#include <QGraphicsSceneHoverEvent>


CellItem::CellItem()
{
    setAcceptHoverEvents(true);
}


bool CellItem::isHovered() const
{
    return is_hovered_;
}

CellItem::CellState CellItem::cellState() const
{
    if (cell()->is_closed) {
        if (cell()->has_flag) {
            return CellState::ClosedWithFlag;
        } else {
            return CellState::Closed;
        }
    } else {
        if (cell()->has_flag && cell()->has_mine) {
            return CellState::OpenedMine;
        } else if (cell()->has_flag && !cell()->has_mine) {
            return CellState::MissedFlag;
        } else if (!cell()->has_flag && cell()->has_mine) {
            return CellState::MissedMine;
        } else {
            return CellState::Opened;
        }
    }
}

void CellItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    is_hovered_ = true;
    update();
    event->ignore();
}

void CellItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    is_hovered_ = false;
    update();
    event->ignore();
}