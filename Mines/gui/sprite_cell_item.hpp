#ifndef SPRITE_CELL_ITEM_HPP
#define SPRITE_CELL_ITEM_HPP

#include "cell_item.hpp"

#include <QFont>

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
    static qreal            size();
    static void           setShape(const QPainterPath &shape);

protected:
    virtual QRectF       spriteRect(CellState type) const;
    virtual const QColor textColor(std::size_t mines) const;

private:
    inline static std::unique_ptr<QPixmap>  sprites_   = nullptr;
    inline static qreal                     size_      = 0.;
    inline static qreal                     half_size_ = 0.;
    inline static QPainterPath              shape_;
    inline static constexpr qreal           hovered_opacity_ = 0.3;
    inline static const std::vector<QColor> text_colors_     = { Qt::black,        { 48, 115, 221 }, { 124, 43, 76 },
                                                                 { 64, 19, 178 },  { 178, 43, 19 },  { 141, 48, 221 },
                                                                 { 19, 178, 175 }, { 47, 58, 4 },    { 53, 18, 31 },
                                                                 { 43, 29, 26 } };
    inline static const QFont               font_            = { "Calibri", 20, QFont::Bold };
};

#endif // SPRITE_CELL_ITEM_HPP
