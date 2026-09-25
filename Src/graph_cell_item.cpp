#include "graph_cell_item.hpp"
#include "cell.hpp"
#include "graph_cell.hpp"

#include <QGraphicsDropShadowEffect>
#include <QPainter>

const QPen GraphCellItem::hovered_pen_{Qt::red};

GraphCellItem::GraphCellItem(const GraphCell* cell)
    : BuddyNotificator{static_cast<QGraphicsItem&>(*this)}, SpriteCellItem{cell}, graph_cell_{cell}
{
}

QRectF GraphCellItem::boundingRect() const
{
    return SpriteCellItem::boundingRect().adjusted(-glow_radius_, -glow_radius_, glow_radius_, glow_radius_);
}

void GraphCellItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    if (isHovered()) {
        QRadialGradient gradient{boundingRect().center(), boundingRect().width() / 2.0 + glow_radius_};
        gradient.setColorAt(0.0, QColor(255, 0, 0, 100));
        gradient.setColorAt(0.6, QColor(255, 0, 0, 40));
        gradient.setColorAt(1.0, QColor(255, 0, 0, 0));

        painter->save();
        painter->setPen(Qt::NoPen);
        painter->setBrush(gradient);
        painter->drawEllipse(boundingRect());
        painter->restore();
    }

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
        buddy->buddyHoveringStateChanged(isHovered());
    }
}

void GraphCellItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    setZValue(zValue() - 1);
    SpriteCellItem::hoverLeaveEvent(event);
    for (auto& buddy : buddies_) {
        buddy->buddyHoveringStateChanged(isHovered());
    }
}
