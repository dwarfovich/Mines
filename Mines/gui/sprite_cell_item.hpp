#ifndef SPRITE_CELL_ITEM_HPP
#define SPRITE_CELL_ITEM_HPP

#include "cell_item.hpp"

class Cell;

class SpriteCellItem : public CellItem
{
public:
    enum
    {
        Type = UserType + 2
    };

    explicit SpriteCellItem(const Cell *cell);

    QPainterPath shape() const override;
    QRectF       boundingRect() const override;
    void         paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    static const QPixmap &sprites();
    static void           setSprites(const QString &path);
    static int            size();
    static void           setShape(const QPainterPath &shape);

protected:
    virtual QRectF       spriteRect(CellState type) const;
    virtual const QColor textColor(size_t mines) const;

private:
    static std::unique_ptr<QPixmap>  sprites_;
    static int                       size_;
    static QPainterPath              shape_;
    static const qreal               hovered_opacity_;
    static const std::vector<QColor> text_colors_;
    static const QFont               font_;
};

#endif // SPRITE_CELL_ITEM_HPP
