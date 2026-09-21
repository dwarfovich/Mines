#pragma once

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

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    void addBuddy(BuddyNotificator* buddy);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

private:
    static const QPen              hovered_pen_;
    std::vector<BuddyNotificator*> buddies_;
    QGraphicsDropShadowEffect*     effect_ = nullptr;
};
