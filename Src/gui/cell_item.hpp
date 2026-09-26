#pragma once

#include "../cell.hpp"

#include <QGraphicsItem>

class CellItem : public QGraphicsItem {
public:
    enum {
        Type = UserType + 1
    };

    enum CellState {
        Closed,
        ClosedWithFlag,
        OpenedMine,
        MissedFlag,
        MissedMine,
        Opened
    };

    CellItem();
    CellItem(const Cell* cell);

    virtual std::size_t cellId() const = 0;
    bool            isHovered() const;

protected:  // methods
    CellItem::CellState cellState() const;
    void                hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void                hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

private:
    bool is_hovered_ = false;
};

