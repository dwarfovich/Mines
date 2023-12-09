#pragma once

#include "edge.hpp"
#include "gui/buddy_notificator.hpp"

#include <QGraphicsItem>

class EdgeItem : public QGraphicsItem, public BuddyNotificator
{
public:
    EdgeItem();
    EdgeItem(const Edge& edge);

    QRectF boundingRect() const override;
    void   paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    static const QPen hovered_pen_;
    Edge              edge_;
};
