#pragma once

#include <QGraphicsDropShadowEffect>
#include "buddy_notificator.hpp"
#include "gui/sprite_cell_item.hpp"
class Cell;
class QGraphicsDropShadowEffect;

class GraphCellItem : public SpriteCellItem, public BuddyNotificator {
public:
    enum {
        Type = UserType + 3
    };

    explicit GraphCellItem(const Cell* cell);

    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    void addBuddy(BuddyNotificator* buddy);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

private:
    static const QPen      hovered_pen_;
    static constexpr qreal glow_radius_ = 15.;

    std::vector<BuddyNotificator*> buddies_;
};
