#include "rectangle_board.hpp"
#include "gui/rectangle_board_parameters_widget.hpp"
#include "gui/rectangle_board_cell_item.hpp"
#include "gui/standard_cell_item.hpp"
#include "gui/board_scene.hpp"

#include <QPainterPath>

#include <QDebug>
#define DEB qDebug()

std::unique_ptr<IBoard> RectangleBoard::clone() const
{
    return std::make_unique<RectangleBoard>();
}

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

void RectangleBoard::drawBoard(BoardScene *scene)
{
    StandardCellItem::setSprites(":/gfx/cells_square.png");
    int sprite_size = StandardCellItem::size();
    QPainterPath path;
    path.moveTo(0., 0.);
    path.lineTo(sprite_size, 0.);
    path.lineTo(sprite_size, sprite_size);
    path.lineTo(0., sprite_size);
    path.closeSubpath();
    StandardCellItem::setShape(path);
    for (size_t i = 0; i < height_; ++i) {
        for (size_t j = 0; j < width_; ++j) {
            auto* item = new StandardCellItem {cellById(i * width_ + j)};
            item->setPos(sprite_size * j, sprite_size * i);
            scene->registerCellItem(item);
        }
    }
}

std::vector<size_t> RectangleBoard::neighborIds(size_t id) const
{
    size_t min_id = (id / width_ > 0 ? id - width_ : id);
    if (min_id % width_ != 0)
        --min_id;
    size_t col_span = ( (id % width_ == 0) || ((id + 1) % width_ == 0)     ? 2 : 3 );
    size_t row_span = ( (id / width_ == 0) || (id / width_ == height_ - 1) ? 2 : 3 );

    std::vector<size_t> ids (col_span * row_span - 1);
    size_t neighbors_counter = 0;
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


QObject *RectangleBoard::toQObject()
{
    return this;
}
