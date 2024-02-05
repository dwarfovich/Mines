#include "polymino_cell_item.hpp"
#include "polymino_cell.hpp"

#include <QPainter>

int PolyminoCellItem::size_ = 0;

void PolyminoCellItem::setSize(int size)
{
    size_ = size;
}

QRectF PolyminoCellItem::boundingRect() const
{
    return bounding_rect_;
}

void PolyminoCellItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QBrush brush { color_ };
    QPen   pen { color_ };
    painter->setPen(pen);
    painter->setBrush(brush);
    //painter->drawPolygon(polygon_);
}

void PolyminoCellItem::initialize(PolyminoCell* cell, const QColor& color)
{
    setCell(cell);
    color_ = color;

    setPos(cell->center.x() * size_, cell->center.y() * size_);

    for (const auto& shift : cell->shifts) {
        children_.push_back(QGraphicsRectItem())
    }
    polygon_.append({ 10, 0 });
    polygon_.append({ 10, 10 });
    polygon_.append({ 0, 10 });

    bounding_rect_ = QRectF { 0, 0, 100, 100 };
}
