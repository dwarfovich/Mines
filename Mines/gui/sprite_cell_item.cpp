#include "sprite_cell_item.hpp"
#include "cell.hpp"

#include <QPainter>
#include <QGraphicsSceneHoverEvent>

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
    half_size_ = size_ / 2.;
}

qreal SpriteCellItem::size()
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
    return { -half_size_, -half_size_, size_, size_ };
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
        painter->drawText(-half_size_, -half_size_, size_, size_, Qt::AlignCenter, QString::number(cell_->neighbor_mines));
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

const QColor SpriteCellItem::textColor(std::size_t mines) const
{
    if (mines < text_colors_.size()) {
        return text_colors_[mines];
    } else {
        return text_colors_.back();
    }
}