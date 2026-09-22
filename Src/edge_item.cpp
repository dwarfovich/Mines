#include "edge_item.hpp"
#include "graph_boards_constants.hpp"
#include "gui/sprite_cell_item.hpp"

#include <QPainter>

const QPen EdgeItem::hovered_pen_{Qt::red};

EdgeItem::EdgeItem() : BuddyNotificator{static_cast<QGraphicsItem&>(*this)} {}

EdgeItem::EdgeItem(const Edge& edge) : BuddyNotificator{static_cast<QGraphicsItem&>(*this)}, edge_{edge} {}

void EdgeItem::setPointItem1(QGraphicsItem* item)
{
    Q_ASSERT(item);

    p1_ = item;
}

void EdgeItem::setPointItem2(QGraphicsItem* item)
{
    Q_ASSERT(item);

    p2_ = item;
}

QRectF EdgeItem::boundingRect() const
{
    auto left = std::min(edge_[0].x(), edge_[1].x());
    auto right = std::max(edge_[0].x(), edge_[1].x());
    auto top = std::min(edge_[0].y(), edge_[1].y());
    auto bottom = std::max(edge_[0].y(), edge_[1].y());

    return {left, top , right - left, bottom - top };
}

void EdgeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    if (buddyIsHovered()) {
        painter->setPen(hovered_pen_);
    } 
    painter->drawLine(QLineF{edge_[0], edge_[1]});
}

void EdgeItem::advance(int step)
{
    Q_ASSERT(p1_);
    Q_ASSERT(p2_);

    if (step == 0) {
        prepareGeometryChange();
        edge_[0] = {p1_->x(), p1_->y()};
        edge_[1] = {p2_->x(), p2_->y()};
    } else {
        update();
    }
}
