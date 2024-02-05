#include "polymino_board.hpp"
#include "polymino_parameters_widget.hpp"
#include "direction.hpp"
#include "qpoint_hasher.hpp"
#include "polymino_cell_item.hpp"
#include "gui/sprite_cell_item.hpp"
#include "gui/board_scene.hpp"

#include <unordered_set>
#include <queue>

#include <iostream>

// NminoBoard::NminoBoard() : random_generator_ { random_device_() }
PolyminoBoard::PolyminoBoard() : random_generator_ { 1 }
{
}

const QString& PolyminoBoard::id() const
{
    static const QString id = "Polymino";
    return id;
}

const QString& PolyminoBoard::name() const
{
    static const QString name = "Polymino";
    return name;
}

void PolyminoBoard::generate()
{
    const size_t width          = 5;
    const size_t height         = 5;
    const size_t max_nmino_size = 5;
    width_                      = width;
    height_                     = height;

    std::uniform_int_distribution<size_t> size_distribution(1, max_nmino_size);

    cells_.clear();
    std::unordered_map<size_t, std::unordered_set<size_t>> neighbors;

    std::vector<std::vector<size_t>> matrix;
    matrix.resize(width, std::vector(height, empty_matrix_id_));
    size_t id = 1;
    for (size_t row = 0; row < height; ++row) {
        for (size_t col = 0; col < width; ++col) {
            if (matrix[row][col] != empty_matrix_id_) {
                continue;
            }

            auto cell                                  = std::make_unique<PolyminoCell>();
            cell->id                                   = id;
            cell->center                               = { static_cast<int>(col), static_cast<int>(row) };
            matrix[cell->center.y()][cell->center.x()] = id;

            const auto         target_size  = size_distribution(random_generator_);
            size_t             current_size = 1;
            std::deque<QPoint> empty_neighbors;
            addEmptyNeighborCells(matrix, cell->center, empty_neighbors);
            while (current_size < target_size && !empty_neighbors.empty()) {
                std::uniform_int_distribution<size_t> distribution(0, empty_neighbors.size() - 1);
                auto                                  neighbor_point = empty_neighbors[distribution(random_generator_)];
                matrix[neighbor_point.y()][neighbor_point.x()]       = id;
                cell->shifts.push_back(neighbor_point - cell->center);
                ++current_size;
                addEmptyNeighborCells(matrix, neighbor_point, empty_neighbors);
            }
            //cell->color = generateRandomColor();
            cells_.push_back(std::move(cell));
            ++id;
        }
    }
}

void PolyminoBoard::setupScene(BoardScene* scene)
{
    PolyminoCellItem::setSize(SpriteCellItem::size());
    for (const auto& cell : cells_) {
        auto item = new PolyminoCellItem();
        item->initialize(cell.get(), generateRandomColor());
        //item->setPos(x, y);
        scene->registerCellItem(item);
    }

    scene->setSceneRect({ 0.,
                          0.,
                          static_cast<qreal>(width_ * SpriteCellItem::size()),
                          static_cast<qreal>(height_ * SpriteCellItem::size()) });

    //SpriteCellItem::setSprites(":/gfx/cells_hex.png");
    //int          sprite_size = SpriteCellItem::size();
    //QPainterPath path;
    //path.moveTo(sprite_size / 2., 0);                // Top
    //path.lineTo(sprite_size, sprite_size / 3.);      // Top right
    //path.lineTo(sprite_size, 2. * sprite_size / 3.); // Bottom right
    //path.lineTo(sprite_size / 2., sprite_size);      // Bottom
    //path.lineTo(0., 2. * sprite_size / 3.);          // Bottom left
    //path.lineTo(0, sprite_size / 3.);                // Top left
    //path.closeSubpath();
    //SpriteCellItem::setShape(path);
    //const size_t cols = width_;
    //for (size_t i = 0; i < height_; ++i) {
    //    for (size_t j = 0; j < width_; ++j) {
    //        const auto& cell        = cells_[i * width_ + j];
    //        auto*       item        = new SpriteCellItem { cell.get() };
    //        qreal       row         = cell->id / cols;
    //        qreal       col         = cell->id % cols;
    //        double      sprite_size = SpriteCellItem::size();
    //        qreal       x = (size_t(row) % 2 == 0 ? col * sprite_size : col * sprite_size + sprite_size * 0.5);
    //        qreal       y = row * (2 * sprite_size / 3);
    //        item->setPos(x, y);
    //        scene->registerCellItem(item);
    //    }
    //}

    //scene->setSceneRect(0, 0, width_ * sprite_size + sprite_size / 2, height_ * sprite_size);
}

QWidget* PolyminoBoard::parametersWidget() const
{
    if (!parameters_widget_) {
        parameters_widget_ = new PolyminoParametersWidget { &dummy_parent_widget_ };
    }

    return parameters_widget_;
}

std::vector<size_t> PolyminoBoard::neighborIds(size_t id) const
{
    return std::vector<size_t>();
}

bool PolyminoBoard::isValidMatrixCoordinates(const QPoint& point, size_t width, size_t height) const
{
    return point.x() >= 0 && point.y() >= 0 && point.x() < width && point.y() < height;
}

QColor PolyminoBoard::generateRandomColor() const
{
    std::random_device random_device;
    // std::mt19937                         random_generator(random_device());
    std::mt19937                         random_generator(1);
    std::uniform_int_distribution<short> color(0, 255);

    return { color(random_generator), color(random_generator), color(random_generator) };
}

bool PolyminoBoard::isEmptyCell(const std::vector<std::vector<size_t>>& matrix, const QPoint& point) const
{
    return isValidMatrixCoordinates(point, matrix[0].size(), matrix.size())
           && matrix[point.y()][point.x()] == empty_matrix_id_;
}

void PolyminoBoard::addEmptyNeighborCells(const std::vector<std::vector<size_t>>& matrix,
                                        const QPoint&                           point,
                                        std::deque<QPoint>&                     neighbors) const
{
    Direction d = Direction::Up;
    do {
        auto shift    = directionToShift(d);
        auto neighbor = point + shift;
        if (isEmptyCell(matrix, neighbor)) {
            neighbors.push_back(neighbor);
        }
        d = nextDirection(d);
    } while (d != Direction::Up);
}
