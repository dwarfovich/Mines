#pragma once

#include "cell.hpp"

#include <QPoint>

#include <vector>

class NminoCell : public Cell
{
public:
    std::vector<QPoint> square_coordinates;
    std::vector<size_t> neighbor_ids;
    size_t              row;
    size_t              col;
};
