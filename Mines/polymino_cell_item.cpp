#include "polymino_cell_item.hpp"
#include "polymino_cell.hpp"
#include "direction.hpp"
#include "utils.hpp"

#include <QPainter>
#include <QGraphicsRectItem>

int PolyminoCellItem::size_ = 0;

void PolyminoCellItem::setSize(int size)
{
    size_ = size;
}

QPainterPath PolyminoCellItem::shape() const
{
    return painter_path_;
}

QRectF PolyminoCellItem::boundingRect() const
{
    return bounding_rect_;
}

void PolyminoCellItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);


    for (const auto& child : children_) {
        child->paint(painter, option, widget);
    }
}

void PolyminoCellItem::initialize(PolyminoCell* cell, const QColor& color)
{
    setCell(cell);
    color_ = color;

    setPos(cell->center.x() * size_, cell->center.y() * size_);

    for (const auto& shift : cell->shifts) {
        auto  x1    = shift.x() * size_;
        auto  y1    = shift.y() * size_;
        auto* child = new QGraphicsRectItem(x1, y1, size_, size_, this);
        child->setBrush(color_);
        children_.push_back(child);
        painter_path_.addRect(child->boundingRect());
        bounding_rect_ = painter_path_.boundingRect();
    }
        int t          = 43;

}
