#include "gui/sprite_cell_item.hpp"

class SimpleSpriteItem : public SpriteCellItem {
public:
    SimpleSpriteItem(const Cell* cell) : cell_{cell}
    {
        Q_ASSERT(cell);
    }

    std::size_t cellId() const override
    {
        return cell_->id;
    }

private:
    const Cell* cell_ = nullptr;
};