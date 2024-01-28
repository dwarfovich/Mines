#pragma once

#include "cell.hpp"

#include <QPoint>
#include <QColor>

#include <vector>

class NminoCell : public Cell
{
public:
    std::vector<QPoint> shifts;
    std::vector<size_t> neighbor_ids;
    QPoint              center;
    QColor              color;
};
