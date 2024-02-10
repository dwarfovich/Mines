#pragma once

#include "cell.hpp"

#include <QPoint>
#include <QColor>

#include <vector>

class PolyominoCell : public Cell
{
public:
    PolyominoCell() = default;
    PolyominoCell(size_t new_id);
    PolyominoCell(size_t new_id, const QPoint& new_center);

    std::vector<QPoint> shifts;
    std::vector<size_t> neighbor_ids;
    QPoint              center;
};
