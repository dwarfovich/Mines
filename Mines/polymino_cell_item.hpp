#pragma once

#include "gui/cell_item.hpp"

#include <QGraphicsRectItem>

class PolyminoCell;

class PolyminoCellItem : public CellItem
{
public:
    PolyminoCellItem() = default;

    QRectF boundingRect() const override;
    void   paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    void   initialize(PolyminoCell* cell, const QColor& color);

    static void setSize(int size);

private:
    static int size_;
    QPolygon   polygon_;
    QRectF     bounding_rect_;
    QColor     color_;
    std::vector<QGraphicsRectItem> children_;
};
