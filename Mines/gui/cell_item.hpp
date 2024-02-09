#ifndef CELLITEM_HPP
#define CELLITEM_HPP

#include <QGraphicsItem>

class Cell;

class CellItem : public QGraphicsItem
{
public:
    enum
    {
        Type = UserType + 1
    };

        enum CellState
    {
        Closed,
        ClosedWithFlag,
        OpenedMine,
        MissedFlag,
        MissedMine,
        Opened
    };

    CellItem() = default;
    CellItem(const Cell* cell);

    const Cell* cell() const;
    void        setCell(Cell* cell);
    bool        IsHovered() const;

protected: // methods
    CellItem::CellState cellState() const;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

protected: // data
    const Cell* cell_ = nullptr;

private:
    bool is_hovered_ = false;
};

#endif // CELLITEM_HPP
