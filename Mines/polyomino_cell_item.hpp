#pragma once

#include "gui/cell_item.hpp"

#include <QBrush>

class PolyominoCell;
class QGraphicsRectItem;

class PolyominoCellItem : public CellItem
{
public:
    QPainterPath shape() const override;
    QRectF       boundingRect() const override;
    void         paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    void         initialize(PolyominoCell* cell, const QColor& color);

private: // methods
    QPainterPath createPainterPath(const PolyominoCell& cell) const;
    QRect        findCellDescriptionRect(const PolyominoCell& cell) const;
    QRectF       spriteRect(CellState state) const;
    void         paintMinesCount(QPainter* painter);
    void         initializeMinesCountAttributes(QPainter* painter);

private: // data
    inline static std::unique_ptr<QPixmap> sprites_       = nullptr;

    QString                                mines_count_;
    int                                    text_x_offset_                     = 0;
    int                                    text_y_offset_                     = 0;
    bool                                   mines_count_attributes_initialized = false;
    QBrush                                 closed_brush_;
    QRectF                                 bounding_rect_;
    QPainterPath                           painter_path_;
    QPolygon                               polygon_;
    QRect                                  cell_info_rect_;
};
