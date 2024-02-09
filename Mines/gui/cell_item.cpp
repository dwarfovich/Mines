#include "cell_item.hpp"
#include "cell.hpp"

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

CellItem::CellState CellItem::cellState() const
{
    if (cell_->is_closed) {
        if (cell_->has_flag) {
            return CellState::ClosedWithFlag;
        } else {
            return CellState::Closed;
        }
    } else {
        if (cell_->has_flag && cell_->has_mine) {
            return CellState::OpenedMine;
        } else if (cell_->has_flag && !cell_->has_mine) {
            return CellState::MissedFlag;
        } else if (!cell_->has_flag && cell_->has_mine) {
            return CellState::MissedMine;
        } else {
            return CellState::Opened;
        }
    }
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