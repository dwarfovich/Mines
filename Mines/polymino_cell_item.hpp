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

private: // methods
    QPainterPath createPainterPath(const PolyminoCell& cell) const;
    QRect        findCellDescriptionRect(const PolyminoCell& cell) const;
    QRectF       spriteRect(CellState state) const
    {
        if (state == CellState::ClosedWithFlag || state == CellState::OpenedMine || state == CellState::MissedFlag
            || state == CellState::MissedMine) {
            return QRectF { size_ * (qreal(state) - 1), 0., qreal(size_), qreal(size_) };
        } else {
            return {};
        }
    }

private: // data
    inline static int                      size_    = 0;
    inline static std::unique_ptr<QPixmap> sprites_ = nullptr;
    QRectF                                 bounding_rect_;
    QPainterPath                           painter_path_;
    QPolygon                               polygon_;
    QColor                                 color_;
    QRect                                  cell_description_rect_;
    std::vector<QGraphicsRectItem*>        children_;
};
