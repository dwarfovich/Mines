#include "cell_item.hpp"

CellItem::CellItem(const Cell *cell)
    : cell_{cell}
{
    setAcceptHoverEvents(true);
}

const Cell *CellItem::cell() const
{
    return cell_;
}

void CellItem::setCell(Cell *cell)
{
    cell_ = cell;
}
