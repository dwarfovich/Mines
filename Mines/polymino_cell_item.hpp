#pragma once

#include "gui/cell_item.hpp"

class PolyminoCell;
class QGraphicsRectItem;

class PolyminoCellItem : public CellItem
{
public:
    PolyminoCellItem() = default;

    QPainterPath shape() const override;
    QRectF       boundingRect() const override;
    void         paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    void         initialize(PolyminoCell* cell, const QColor& color);

    static void setSize(int size);

private: // data
    static int                      size_;
    QRectF                          bounding_rect_;
    QPainterPath                    painter_path_;
    QPolygon polygon_;
    QColor                          color_;
    std::vector<QGraphicsRectItem*> children_;
};
