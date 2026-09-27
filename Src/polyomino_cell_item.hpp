#pragma once

#include "gui/cell_item.hpp"

#include <QBrush>

class PolyominoCell;

class PolyominoCellItem : public CellItem {
public:
    PolyominoCellItem(const PolyominoCell* cell);

    std::size_t  cellId() const override;
    const Cell*  cell() const override;
    QPainterPath shape() const override;
    QRectF       boundingRect() const override;
    void         paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:  // methods
    static QColor generateCellColor();

    void         initialize();
    QPainterPath createPainterPath(const PolyominoCell& cell) const;
    QRect        findCellDescriptionRect(const PolyominoCell& cell) const;
    QRectF       spriteRect(CellState state) const;
    void         paintMinesCount(QPainter* painter);
    void         initializeMinesCountAttributes();

private:  // data
    inline static std::unique_ptr<QPixmap> sprites_ = nullptr;

    const PolyominoCell* cell_ = nullptr;
    QString              mines_count_;
    int                  text_x_offset_ = 0;
    int                  text_y_offset_ = 0;
    bool                 mines_count_attributes_initialized = false;
    QBrush               closed_brush_;
    QRectF               bounding_rect_;
    QPainterPath         painter_path_;
    QPolygon             polygon_;
    QRect                cell_info_rect_;
};
