#include "rectangle_board.hpp"
#include "gui/rectangle_board_parameters_widget.hpp"
#include "gui/sprite_cell_item.hpp"
#include "gui/board_scene.hpp"

#include <QPainterPath>

const QString &RectangleBoard::id() const
{
    static const QString id = "Rectangle";

    return id;
}

const QString &RectangleBoard::name() const
{
    static const QString name = tr("Rectangle");

    return name;
}

void RectangleBoard::setupScene(BoardScene *scene)
{
    SpriteCellItem::setSprites(":/gfx/cells_square.png");
    const auto          sprite_size = SpriteCellItem::size();
    QPainterPath path;
    path.moveTo(0., 0.);
    path.lineTo(sprite_size, 0.);
    path.lineTo(sprite_size, sprite_size);
    path.lineTo(0., sprite_size);
    path.closeSubpath();
    SpriteCellItem::setShape(path);
    for (size_t i = 0; i < height_; ++i) {
        for (size_t j = 0; j < width_; ++j) {
            auto *item = new SpriteCellItem { cellById(i * width_ + j) };
            item->setPos(sprite_size * j, sprite_size * i);
            scene->registerCellItem(item);
        }
    }

    scene->setSceneRect(0, 0, width_ * sprite_size, height_ * sprite_size);
}

void RectangleBoard::generate()
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

QWidget *RectangleBoard::parametersWidget() const
{
    if (!parameters_widget_) {
        parameters_widget_ = new RectangleBoardParametersWidget { &dummy_parent_widget_ };
    }

    return parameters_widget_;
}

std::vector<size_t> RectangleBoard::neighborIds(size_t id) const
{
    size_t min_id = (id / width_ > 0 ? id - width_ : id);
    if (min_id % width_ != 0) {
        --min_id;
    
    }
    size_t col_span = ((id % width_ == 0) || ((id + 1) % width_ == 0) ? 2 : 3);
    size_t row_span = ((id / width_ == 0) || (id / width_ == height_ - 1) ? 2 : 3);

    std::vector<size_t> ids(col_span * row_span - 1);
    size_t              neighbors_counter = 0;
    while (row_span != 0) {
        for (size_t j = 0; j < col_span; ++j, ++min_id) {
            if (min_id != id) {
                ids[neighbors_counter++] = min_id;
            }
        }
        min_id += width_ - col_span;
        --row_span;
    }

    return ids;
}