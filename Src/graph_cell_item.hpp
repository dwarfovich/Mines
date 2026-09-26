#pragma once

#include "buddy_notificator.hpp"
#include "gui/sprite_cell_item.hpp"
#include "graph_cell.hpp"

class GraphCell;
class Cell;
class QGraphicsDropShadowEffect;

class GraphCellItem : public SpriteCellItem, public BuddyNotificator {
public:
    enum {
        Type = UserType + 3
    };

    explicit GraphCellItem(const GraphCell* cell);
    std::size_t cellId() const override
    {
        return graph_cell_->id;
    }
    QRectF boundingRect() const;
    void   paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    void   addBuddy(BuddyNotificator* buddy);

protected:  // methods
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

protected:  // data
    const GraphCell* graph_cell_ = nullptr;

private:
    static const QPen      hovered_pen_;
    static constexpr qreal glow_radius_ = 15.;

    std::vector<BuddyNotificator*> buddies_;
};
