#include "gui/sprite_cell_item.hpp"

class Cell;

class SimpleSpriteItem : public SpriteCellItem {
public:
    SimpleSpriteItem(const Cell* cell);

    std::size_t cellId() const override;
    const Cell* cell() const override;

private:
    const Cell* cell_ = nullptr;
};