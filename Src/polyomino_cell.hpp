#pragma once

#include "cell.hpp"

#include <QColor>
#include <QPoint>

#include <vector>

class PolyominoCell : public Cell {
public:
    PolyominoCell() = default;
    PolyominoCell(std::size_t new_id);
    PolyominoCell(std::size_t new_id, const QPoint& new_center);

    std::vector<QPoint>      shifts;
    std::vector<std::size_t> neighbor_ids;
    QPoint                   center;
};
