#include "graph_cell_item.hpp"
#include "cell.hpp"

#include <QPainter>
#include <QGraphicsDropShadowEffect>

const QPen GraphCellItem::hovered_pen_ { Qt::red };

GraphCellItem::GraphCellItem(const Cell* cell)
    : BuddyNotificator { static_cast<QGraphicsItem&>(*this) }, SpriteCellItem { cell }
{
    effect_ = new QGraphicsDropShadowEffect();
    effect_->setOffset(0, 0);
    effect_->setBlurRadius(size() * 2);
    effect_->setColor(Qt::red);
    setGraphicsEffect(effect_);
}

void GraphCellItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    effect_->setEnabled(BuddyIsHovered());
    SpriteCellItem::paint(painter, option, widget);
}

void GraphCellItem::addBuddy(BuddyNotificator* buddy)
{
    buddies_.push_back(buddy);
}

void GraphCellItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    setZValue(zValue() + 1);
    SpriteCellItem::hoverEnterEvent(event);
    for (auto& buddy : buddies_) {
        buddy->BuddyHoveringStateChanged(IsHovered());
    }
}

void GraphCellItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    setZValue(zValue() - 1);
    SpriteCellItem::hoverLeaveEvent(event);
    for (auto& buddy : buddies_) {
        buddy->BuddyHoveringStateChanged(IsHovered());
    }
}
