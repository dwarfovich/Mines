#pragma once

#include "../cell.hpp"

#include <QGraphicsItem>

class CellItem : public QGraphicsItem {
public:
    enum {
        Type = UserType + 1
    };

    CellItem();
    CellItem(const Cell* cell);

    int                 type() const override;
    virtual std::size_t cellId() const = 0;
    virtual const Cell* cell() const = 0;
    bool                isHovered() const;

protected:  // methods
    enum CellState {
        Closed,
        ClosedWithFlag,
        OpenedMine,
        MissedFlag,
        MissedMine,
        Opened
    };

    CellItem::CellState cellState() const;
    void                hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void                hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

private:
    bool is_hovered_ = false;
};
