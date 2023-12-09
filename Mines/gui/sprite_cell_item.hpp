#ifndef STANDARDCELLITEM_HPP
#define STANDARDCELLITEM_HPP

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

    static const QPixmap &sprites();
    static void           setSprites(const QString &path);
    static int            size();
    static void           setShape(const QPainterPath &shape);
    QPainterPath          shape() const override;
    QRectF                boundingRect() const override;
    void                  paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    bool                 IsHovered() const;
    virtual SpriteType   spriteTypeForCurrentCell() const;
    virtual QRectF       spriteRect(SpriteType type) const;
    virtual const QColor textColor(size_t mines) const;
    void                 hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void                 hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    static std::unique_ptr<QPixmap>  sprites_;
    static int                       size_;
    static QPainterPath              shape_;
    static const qreal               hovered_opacity_;
    static const std::vector<QColor> text_colors_;
    static const QFont               font_;
    bool                             is_hovered_ = false;
};

#endif // STANDARDCELLITEM_HPP
