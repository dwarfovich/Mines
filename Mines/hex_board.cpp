#include "hex_board.hpp"
#include "hex_parameters_widget.hpp"
#include "gui/board_scene.hpp"
#include "gui/sprite_cell_item.hpp"

#include <numbers>

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

void HexBoard::setupScene(BoardScene *scene)
{
    SpriteCellItem::setSprites(":/gfx/cells_hex.png");
    const auto sprite_size      = SpriteCellItem::size();
    const auto half_sprite_size = sprite_size / 2.;
    using namespace std::numbers;
    QPainterPath path;
    path.moveTo(0., -half_sprite_size);                                             // Top
    path.lineTo(half_sprite_size, -half_sprite_size + (half_sprite_size / sqrt3));  // Top right
    path.lineTo(half_sprite_size, half_sprite_size - (half_sprite_size / sqrt3));   // Bottom right
    path.lineTo(0, half_sprite_size);                                               // Bottom
    path.lineTo(-half_sprite_size, half_sprite_size - (half_sprite_size / sqrt3));  // Bottom left
    path.lineTo(-half_sprite_size, -half_sprite_size + (half_sprite_size / sqrt3)); // Top left
    path.closeSubpath();
    SpriteCellItem::setShape(path);
    const size_t cols = width_;
    for (size_t i = 0; i < height_; ++i) {
        for (size_t j = 0; j < width_; ++j) {
            const auto &cell = cells_[i * width_ + j];
            auto       *item = new SpriteCellItem { cell.get() };
            const qreal x    = (i % 2 == 0 ? static_cast<qreal>(j) * sprite_size
                                           : static_cast<qreal>(j) * sprite_size + half_sprite_size);
            const qreal y    = static_cast<qreal>(i) * (sprite_size - (half_sprite_size / sqrt3));
            item->setPos(x, y);
            scene->registerCellItem(item);
        }
    }

    scene->setSceneRect(-half_sprite_size,
                        -half_sprite_size,
                        width_ * sprite_size + half_sprite_size,
                        height_ * (sprite_size - (half_sprite_size / sqrt3)) + half_sprite_size / sqrt3);
}

void HexBoard::generate()
{
    if (!parameters_widget_) {
        Q_ASSERT(false);
        return;
    }

    width_                   = parameters_widget_->boardWidth();
    height_                  = parameters_widget_->boardHeight();
    board_state_             = {};
    flags_                   = 0;
    board_state_.mines       = parameters_widget_->mines();
    board_state_.empty_cells = width_ * height_ - board_state_.mines;

    initializeCells(width_ * height_);
    randomize();

    board_state_.game_state = GameState::Playing;
}

QWidget *HexBoard::parametersWidget() const
{
    if (!parameters_widget_) {
        parameters_widget_ = new HexParametersWidget { &dummy_parent_widget_ };
    }

    return parameters_widget_;
}

std::vector<size_t> HexBoard::neighborIds(size_t id) const
{
    const size_t        max_neighbors = 6;
    std::vector<size_t> ids(max_neighbors);
    size_t              neighbors_counter = 0;
    size_t              col               = id % width_;
    size_t              row               = id / width_;
    if (col > 0) {
        ids[neighbors_counter++] = id - 1;
    }
    if (col < width_ - 1) {
        ids[neighbors_counter++] = id + 1;
    }
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
