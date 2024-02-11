#pragma once

#include "edge.hpp"
#include "buddy_notificator.hpp"

#include <QGraphicsItem>

class EdgeItem : public QGraphicsItem, public BuddyNotificator
{
public:
    enum
    {
        Type = UserType + 4
    };

    EdgeItem();
    EdgeItem(const Edge& edge);

    void   setPointItem1(QGraphicsItem* item);
    void   setPointItem2(QGraphicsItem* item);
    QRectF boundingRect() const override;
    void   paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
    void advance(int step) override;

private:
    static const QPen hovered_pen_;
    Edge              edge_;
    QGraphicsItem*    p1_ = nullptr;
    QGraphicsItem*    p2_ = nullptr;
};
