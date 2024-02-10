#include "sprite_cell_item.hpp"
#include "cell.hpp"

#include <QPainter>
#include <QGraphicsSceneHoverEvent>

std::unique_ptr<QPixmap>  SpriteCellItem::sprites_         = nullptr;
int                       SpriteCellItem::size_            = 0;
QPainterPath              SpriteCellItem::shape_           = {};
const qreal               SpriteCellItem::hovered_opacity_ = 0.3;
const QFont               SpriteCellItem::font_ { "Calibri", 20, QFont::Bold };
const std::vector<QColor> SpriteCellItem::text_colors_ = { Qt::black,        { 48, 115, 221 }, { 124, 43, 76 },
                                                           { 64, 19, 178 },  { 178, 43, 19 },  { 141, 48, 221 },
                                                           { 19, 178, 175 }, { 47, 58, 4 },    { 53, 18, 31 },
                                                           { 43, 29, 26 } };

SpriteCellItem::SpriteCellItem(const Cell *cell) : CellItem { cell }
{
}

const QPixmap &SpriteCellItem::sprites()
{
    return *sprites_;
}

void SpriteCellItem::setSprites(const QString &path)
{
    sprites_ = std::make_unique<QPixmap>(path);
    size_    = sprites_->height();
}

int SpriteCellItem::size()
{
    return size_;
}

QPainterPath SpriteCellItem::shape() const
{
    return shape_;
}

void SpriteCellItem::setShape(const QPainterPath &shape)
{
    shape_ = shape;
}

QRectF SpriteCellItem::boundingRect() const
{
    return { 0, 0, qreal(size_), qreal(size_) };
}

void SpriteCellItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    Q_ASSERT(cell_);

    auto cell_state = cellState();
    painter->drawPixmap(boundingRect(), *SpriteCellItem::sprites_, spriteRect(cell_state));
    if (cell_state == CellState::Opened && cell_->neighbor_mines > 0) {
        painter->setFont(font_);
        painter->setPen(textColor(cell_->neighbor_mines));
        painter->drawText(0., 0., size_, size_, Qt::AlignCenter, QString::number(cell_->neighbor_mines));
    }

    if (IsHovered()) {
        painter->setBrush(Qt::white);
        painter->setPen(Qt::NoPen);
        painter->setOpacity(hovered_opacity_);
        painter->drawPath(shape());
    }
}

QRectF SpriteCellItem::spriteRect(CellState type) const
{
    return QRectF { size_ * (qreal(type)), 0., qreal(size_), qreal(size_) };
}

const QColor SpriteCellItem::textColor(size_t mines) const
{
    if (mines < text_colors_.size()) {
        return text_colors_[mines];
    } else {
        return text_colors_.back();
    }
}