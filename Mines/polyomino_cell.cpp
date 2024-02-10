#include "polyomino_cell.hpp"

PolyominoCell::PolyominoCell(size_t new_id) : Cell {new_id}
{
}

PolyominoCell::PolyominoCell(size_t new_id, const QPoint& new_center) : Cell { new_id }, center {new_center}
{
}
