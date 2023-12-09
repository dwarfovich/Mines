#include "hex_board.hpp"
#include "gui/board_scene.hpp"
#include "gui/sprite_cell_item.hpp"

std::unique_ptr<IBoard> HexBoard::create() const
{
    return std::make_unique<HexBoard>();
}

QObject *HexBoard::toQObject()
{
    return this;
}

const QString &HexBoard::id() const
{
    static const QString id { "Hex" };
    return id;
}

const QString &HexBoard::name() const
{
    static const QString name = tr("Hex");
    return name;
}

void HexBoard::drawBoard(BoardScene *scene)
{
    SpriteCellItem::setSprites(":/gfx/cells_hex.png");
    int          sprite_size = SpriteCellItem::size();
    QPainterPath path;
    path.moveTo(sprite_size / 2., 0);                // Top
    path.lineTo(sprite_size, sprite_size / 3.);      // Top right
    path.lineTo(sprite_size, 2. * sprite_size / 3.); // Bottom right
    path.lineTo(sprite_size / 2., sprite_size);      // Bottom
    path.lineTo(0., 2. * sprite_size / 3.);          // Bottom left
    path.lineTo(0, sprite_size / 3.);                // Top left
    path.closeSubpath();
    SpriteCellItem::setShape(path);
    const size_t cols = width_;
    for (size_t i = 0; i < height_; ++i) {
        for (size_t j = 0; j < width_; ++j) {
            const auto *cell        = &cells_[i * width_ + j];
            auto       *item        = new SpriteCellItem { cell };
            qreal       row         = cell->id / cols;
            qreal       col         = cell->id % cols;
            double      sprite_size = SpriteCellItem::size();
            qreal       x = (size_t(row) % 2 == 0 ? col * sprite_size : col * sprite_size + sprite_size * 0.5);
            qreal       y = row * (2 * sprite_size / 3);
            item->setPos(x, y);
            scene->registerCellItem(item);
        }
    }
}

std::vector<size_t> HexBoard::neighborIds(size_t id) const
{
    const size_t        max_neighbors = 6;
    std::vector<size_t> ids(max_neighbors);
    size_t              neighbors_counter = 0;
    size_t              col               = id % width_;
    size_t              row               = id / width_;
    if (col > 0)
        ids[neighbors_counter++] = id - 1;
    if (col < width_ - 1)
        ids[neighbors_counter++] = id + 1;
    if (row % 2 == 0) { // Even row
        if (row > 0 && col > 0) {
            ids[neighbors_counter++] = id - width_ - 1;
        }
        if (row > 0) {
            ids[neighbors_counter++] = id - width_;
        }
        if (row < height_ - 1 && col > 0) {
            ids[neighbors_counter++] = id + width_ - 1;
        }
        if (row < height_ - 1) {
            ids[neighbors_counter++] = id + width_;
        }
    } else { // Odd row
        if (row > 0 && col < width_ - 1) {
            ids[neighbors_counter++] = id - width_ + 1;
        }
        if (row > 0) {
            ids[neighbors_counter++] = id - width_;
        }
        if (row < height_ - 1 && col < width_ - 1) {
            ids[neighbors_counter++] = id + width_ + 1;
        }
        if (row < height_ - 1) {
            ids[neighbors_counter++] = id + width_;
        }
    }

    ids.resize(neighbors_counter);

    return ids;
}
