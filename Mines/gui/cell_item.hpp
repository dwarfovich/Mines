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
    enum SpriteType
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

    virtual const Cell* cell() const;
    virtual void        setCell(Cell* cell);

protected:
    const Cell* cell_ = nullptr;
};

#endif // CELLITEM_HPP
