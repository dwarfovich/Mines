#include "standard_cell_item.hpp"
#include "cell.hpp"

#include <QPainter>
#include <QGraphicsSceneHoverEvent>

std::unique_ptr<QPixmap> StandardCellItem::sprites_ = nullptr;
int StandardCellItem::size_ = 0;
QPainterPath StandardCellItem::shape_ = {};
const qreal StandardCellItem::hovered_opacity_ = 0.3;
const QFont StandardCellItem::font_ {"Calibri", 20, QFont::Bold};
const std::vector<QColor> StandardCellItem::text_colors_ = {
    Qt::black, {48, 115, 221}, {124, 43, 76},
    {64, 19, 178}, {178, 43, 19}, {141, 48, 221},
    {19, 178, 175}, {47, 58, 4}, {53, 18, 31},
    {43, 29, 26}};

StandardCellItem::StandardCellItem(const Cell *cell)
    : CellItem{cell}
{
    setAcceptHoverEvents(true);
}

const QPixmap& StandardCellItem::sprites()
{
    return *sprites_;
}

void StandardCellItem::setSprites(const QString &path)
{
    sprites_ = std::make_unique<QPixmap>(path);
    size_ = sprites_->height();
}

int StandardCellItem::size()
{
    return size_;
}

QPainterPath StandardCellItem::shape() const
{
    return shape_;
}

void StandardCellItem::setShape(const QPainterPath &shape)
{
    shape_ = shape;
}


QRectF StandardCellItem::boundingRect() const
{
    return {0, 0, qreal(size_), qreal(size_)};
}

void StandardCellItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    Q_ASSERT(cell_);

    auto spriteType = spriteTypeForCurrentCell();
    painter->drawPixmap( boundingRect(), *StandardCellItem::sprites_, spriteRect(spriteType));
    if (spriteType == SpriteType::Opened && cell_->neighbor_mines > 0) {
        painter->setFont(font_);
        painter->setPen(textColor(cell_->neighbor_mines));
        painter->drawText(0., 0., size_, size_,
                          Qt::AlignCenter, QString::number(cell_->neighbor_mines));
    }

    if (is_hovered_) {
        painter->setBrush(Qt::white);
        painter->setPen(Qt::NoPen);
        painter->setOpacity(0.3);
        painter->drawPath(shape());
    }
}

CellItem::SpriteType StandardCellItem::spriteTypeForCurrentCell() const
{
    if(cell_->is_closed) {
        if (cell_->has_flag) {
            return SpriteType::ClosedWithFlag;
        } else {
            return SpriteType::Closed;
        }
    } else {
        if (cell_->has_flag && cell_->has_mine) {
            return SpriteType::OpenedMine;
        } else if (cell_->has_flag && !cell_->has_mine) {
            return SpriteType::MissedFlag;
        } else if (!cell_->has_flag && cell_->has_mine) {
            return SpriteType::MissedMine;
        } else {
            return SpriteType::Opened;
        }
    }
}

QRectF StandardCellItem::spriteRect(SpriteType type) const
{
    return QRectF{size_*(qreal(type)), 0., qreal(size_), qreal(size_)};
}

const QColor StandardCellItem::textColor(size_t mines) const
{
    if (mines < text_colors_.size()) {
        return text_colors_[mines];
    } else {
        return text_colors_.back();
    }
}


void StandardCellItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    is_hovered_ = true;
    update();
    event->ignore();
}

void StandardCellItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    is_hovered_ = false;
    update();
    event->ignore();
}
