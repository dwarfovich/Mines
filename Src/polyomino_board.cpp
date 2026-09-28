#include "polyomino_board.hpp"
#include "direction.hpp"
#include "polyomino_board_constants.hpp"
#include "polyomino_cell_item.hpp"
#include "polyomino_parameters_widget.hpp"

#include "gui/sprite_cell_item.hpp"
#include "gui/board_scene.hpp"

const QString& PolyominoBoard::id() const
{
    static const QString id = "Polyomino";
    return id;
}

const QString& PolyominoBoard::name() const
{
    static const QString name = "Polyomino";
    return name;
}

void PolyominoBoard::generate()
{
    setupBoard();

    IdsMatrix ids_matrix;
    ids_matrix.resize(height_, std::vector(width_, constants::polyomino_board::empty_matrix_id));
    size_t id = 0;
    for (size_t row = 0; row < height_; ++row) {
        for (size_t col = 0; col < width_; ++col) {
            if (ids_matrix[row][col] != constants::polyomino_board::empty_matrix_id) {
                continue;
            }

            auto cell = PolyominoCell(id, QPoint{static_cast<int>(col), static_cast<int>(row)});
            ids_matrix[row][col] = id;
            generatePolyomino(cell, ids_matrix);
            cells_.push_back(std::move(cell));
            ++id;
        }
    }

    board_state_.empty_cells = cells_.size() - board_state_.mines;
    setupNeighbors(ids_matrix);
    setupMines();
}

void PolyominoBoard::setupScene(BoardScene* scene)
{
    for (const auto& cell : cells_) {
        scene->registerCellItem(new PolyominoCellItem(&cell));
    }

    scene->setSceneRect({0.,
                         0.,
                         static_cast<qreal>(width_ * SpriteCellItem::size()),
                         static_cast<qreal>(height_ * SpriteCellItem::size())});
}

std::vector<size_t> PolyominoBoard::neighborIds(size_t id) const
{
    if (id < cells_.size()) {
        return cells_[id].neighbor_ids;
    } else {
        Q_ASSERT(false && "Wrong id");
        return {};
    }
}

void PolyominoBoard::setupBoard()
{
    auto* parameters_widget = parametersWidget();
    Q_ASSERT(parameters_widget);

    width_ = parameters_widget->width();
    height_ = parameters_widget->height();
    max_polyomino_size_ = parameters_widget->maxPolyominoSize();

    board_state_ = {};
    board_state_.mines = parameters_widget->minesCount();
    
    cells_.clear();
}

bool PolyominoBoard::isValidMatrixCoordinates(const QPoint& point, size_t width, size_t height) const
{
    return point.x() >= 0 && point.y() >= 0 && point.x() < width && point.y() < height;
}

void PolyominoBoard::setupMines()
{
    const auto        mines_count = std::min(board_state_.mines, cells_.size() - 1);
    std::vector<bool> mines(mines_count, false);
    for (size_t i = 0; i < mines_count; ++i) {
        mines[i] = true;
    }
    std::shuffle(mines.begin(), mines.end(), random_generator_);

    for (size_t i = 0; i < mines_count; ++i) {
        cells_[i].has_mine = mines[i];
    }
}

bool PolyominoBoard::isEmptyCell(const std::vector<std::vector<size_t>>& matrix, const QPoint& point) const
{
    return isValidMatrixCoordinates(point, matrix[0].size(), matrix.size()) &&
           matrix[point.y()][point.x()] == constants::polyomino_board::empty_matrix_id;
}

void PolyominoBoard::addEmptyNeighborCells(const std::vector<std::vector<size_t>>& matrix,
                                           const QPoint&                           point,
                                           std::vector<QPoint>&                    neighbors) const
{
    for (const auto direction : directions_array) {
        const auto shift = directionToShift(direction);
        const auto neighbor = point + shift;
        if (isEmptyCell(matrix, neighbor)) {
            neighbors.push_back(neighbor);
        }
    }
}

void PolyominoBoard::setupNeighbors(const IdsMatrix& ids)
{
    auto addNeighborsSymmetrically = [this](std::size_t id1, std::size_t id2) {
        auto* cell1 = cellById(id1);
        if (!contains(cell1->neighbor_ids, id2)) {
            cell1->neighbor_ids.push_back(id2);
        }
        auto& cell2 = cells_[id2];
        if (!contains(cell2.neighbor_ids, id1)) {
            cell2.neighbor_ids.push_back(id1);
        }
    };

    for (std::size_t i = 0; i < height_; ++i) {
        for (std::size_t j = 0; j < width_; ++j) {
            const auto  current_id = ids[i][j];
            const auto* cell = cellById(current_id);
            for (const auto& shift : cell->shifts) {
                for (const auto direction : extended_directions_array) {
                    const auto& neighbor_coords = cell->center + shift + directionToShift(direction);
                    if (!isValidMatrixCoordinates(neighbor_coords, width_, height_)) {
                        continue;
                    }
                    auto neighbor_id = ids[neighbor_coords.y()][neighbor_coords.x()];
                    Q_ASSERT(neighbor_id != constants::polyomino_board::empty_matrix_id);
                    if (neighbor_id == current_id) {
                        continue;
                    }
                    addNeighborsSymmetrically(current_id, neighbor_id);
                }
            }
        }
    }
}

void PolyominoBoard::generatePolyomino(PolyominoCell& cell, IdsMatrix& ids)
{
    using SizeTDistribution = std::uniform_int_distribution<size_t>;
    SizeTDistribution size_distribution{1, max_polyomino_size_};
    const auto        target_size = size_distribution(random_generator_);
    cell.shifts.push_back({0, 0});
    size_t              current_size = 1;
    std::vector<QPoint> empty_neighbors;
    addEmptyNeighborCells(ids, cell.center, empty_neighbors);
    while (current_size < target_size && !empty_neighbors.empty()) {
        SizeTDistribution neighbor_distribution{0, empty_neighbors.size() - 1};
        const auto&       next_neighbor_point = empty_neighbors[neighbor_distribution(random_generator_)];
        const auto&       next_neighbor_shift = next_neighbor_point - cell.center;
        if (isEmptyCell(ids, next_neighbor_point) && !contains(cell.shifts, next_neighbor_shift)) {
            cell.shifts.push_back(next_neighbor_shift);
            ids[next_neighbor_point.y()][next_neighbor_point.x()] = cell.id;
            ++current_size;
            addEmptyNeighborCells(ids, next_neighbor_point, empty_neighbors);
        }
        std::erase(empty_neighbors, next_neighbor_point);
    }
}
