#include "graph_cell_item.hpp"

#include <QPainter>

const QPen GraphCellItem::hovered_pen_ { Qt::red };

GraphCellItem::GraphCellItem(const Cell* cell)
    : BuddyNotificator { static_cast<QGraphicsItem&>(*this) }, SpriteCellItem { cell }
{
}

void GraphCellItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    SpriteCellItem::paint(painter, option, widget);
    if (BuddyIsHovered()) {
        painter->setPen(hovered_pen_);
        painter->drawEllipse(0, 0, 10, 10);
    }
}

void GraphCellItem::addBuddy(BuddyNotificator* buddy)
{
    buddies_.push_back(buddy);
}

void GraphCellItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    SpriteCellItem::hoverEnterEvent(event);
    for (auto& buddy : buddies_) {
        buddy->BuddyHoveringStateChanged(IsHovered());
    }
}

void GraphCellItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    SpriteCellItem::hoverLeaveEvent(event);
    for (auto& buddy : buddies_) {
        buddy->BuddyHoveringStateChanged(IsHovered());
    }
}
