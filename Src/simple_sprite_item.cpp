#include "simple_sprite_item.hpp"

    SimpleSpriteItem::SimpleSpriteItem(const Cell* cell) : cell_{cell}
{
    Q_ASSERT(cell);
}

std::size_t SimpleSpriteItem::cellId() const
{
    return cell_->id;
}

const Cell* SimpleSpriteItem::cell() const
{
    return cell_;
}
