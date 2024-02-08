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
    const size_t mines          = 2;

    board_state_             = {};
    board_state_.mines       = mines;
    size_t cells_counter     = cells_.size();
    board_state_.empty_cells = cells_counter - board_state_.mines;

    width_  = width;
    height_ = height;

    std::uniform_int_distribution<size_t> size_distribution { 1, max_nmino_size };

    cells_.clear();

    std::vector<std::vector<size_t>> matrix;
    matrix.resize(height, std::vector(width, empty_matrix_id_));
    size_t id = 0;
    for (size_t row = 0; row < height; ++row) {
        for (size_t col = 0; col < width; ++col) {
            if (matrix[row][col] != empty_matrix_id_) {
                continue;
            }

            auto cell                                  = std::make_unique<PolyminoCell>();
            cell->id                                   = id;
            cell->center                               = { static_cast<int>(col), static_cast<int>(row) };
            matrix[cell->center.y()][cell->center.x()] = id;

            const auto target_size  = size_distribution(random_generator_);
            size_t     current_size = 1;
            cell->shifts.push_back({ 0, 0 });
            std::deque<QPoint> empty_neighbors;
            addEmptyNeighborCells(matrix, cell->center, empty_neighbors);
            static constexpr int maxAttempts = 20;
            int                  attempt     = 0;
            while (current_size < target_size && !empty_neighbors.empty() && ++attempt <= maxAttempts) {
                std::uniform_int_distribution<size_t> distribution { 0, empty_neighbors.size() - 1 };
                auto                                  neighbor_point = empty_neighbors[distribution(random_generator_)];
                matrix[neighbor_point.y()][neighbor_point.x()]       = id;
                if (!::contains(cell->shifts, neighbor_point - cell->center)) {
                    cell->shifts.push_back(neighbor_point - cell->center);
                    ++current_size;
                    addEmptyNeighborCells(matrix, neighbor_point, empty_neighbors);
                }
            }
            setupNeighbors(matrix, *cell);
            cells_.push_back(std::move(cell));
            ++id;
        }
    }

    assignMines(board_state_.mines);
}

void PolyminoBoard::setupScene(BoardScene* scene)
{
    PolyminoCellItem::setSize(cell_square_size_);
    for (const auto& cell : cells_) {
        auto item = new PolyminoCellItem();
        item->initialize(cell.get(), generateRandomColor());
        scene->registerCellItem(item);
    }

    scene->setSceneRect({ 0.,
                          0.,
                          static_cast<qreal>(width_ * SpriteCellItem::size()),
                          static_cast<qreal>(height_ * SpriteCellItem::size()) });
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
    if (id < cells_.size()) {
        return cells_[id]->neighbor_ids;
    } else {
        Q_ASSERT(false && "Wrong id");
        return {};
    }
}

bool PolyminoBoard::isValidMatrixCoordinates(const QPoint& point, size_t width, size_t height) const
{
    return point.x() >= 0 && point.y() >= 0 && point.x() < width && point.y() < height;
}

void PolyminoBoard::setupNeighbors(const std::vector<std::vector<size_t>>& matrix, PolyminoCell& cell)
{
    const auto currentId = cell.id;
    for (const auto& shift : cell.shifts) {
        const auto currentSubCell = cell.center + shift;
        for (const auto direction : directionsArray) {
            const auto neighborCellCoords = currentSubCell + directionToShift(direction);
            if (!isValidMatrixCoordinates(neighborCellCoords, width_, height_)) {
                continue;
            }
            auto neighborId = matrix[neighborCellCoords.y()][neighborCellCoords.x()];
            if (neighborId == currentId || neighborId == empty_matrix_id_) {
                continue;
            }
            if (!::contains(cell.neighbor_ids, neighborId)) {
                cell.neighbor_ids.push_back(neighborId);
            }
            auto& neighborCell = cells_[neighborId];
            if (!::contains(neighborCell->neighbor_ids, currentId)) {
                neighborCell->neighbor_ids.push_back(currentId);
            }
        }
    }
}

void PolyminoBoard::assignMines(size_t minesCount)
{
    std::vector<bool> mines(cells_.size(), false);
    for (size_t i = 0; i < minesCount; ++i) {
        mines[i] = true;
    }
    std::shuffle(mines.begin(), mines.end(), random_generator_);

    for (size_t i = 0; i < cells_.size(); ++i) {
        cells_[i]->has_mine = mines[i];
    }
}

QColor PolyminoBoard::generateRandomColor() const
{
    std::uniform_int_distribution<short> color(0, 255);

    return { color(random_generator_), color(random_generator_), color(random_generator_) };
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
