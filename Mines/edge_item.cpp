#include "edge_item.hpp"

#include <QPainter>

const QPen EdgeItem::hovered_pen_ { Qt::red };

EdgeItem::EdgeItem() : BuddyNotificator { static_cast<QGraphicsItem&>(*this) }
{
}

EdgeItem::EdgeItem(const Edge& edge) : BuddyNotificator { static_cast<QGraphicsItem&>(*this) }, edge_ { edge }
{
}

QRectF EdgeItem::boundingRect() const
{
    auto left   = std::min(edge_[0].x(), edge_[1].x());
    auto right  = std::max(edge_[0].x(), edge_[1].x());
    auto top    = std::min(edge_[0].y(), edge_[1].y());
    auto bottom = std::max(edge_[0].y(), edge_[1].y());
    return { left, top, right, bottom };
}

void EdgeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    if (BuddyIsHovered()) {
        painter->setPen(hovered_pen_);
    }
    painter->drawLine(edge_[0], edge_[1]);
}
