#include "polyomino_board.hpp"
#include "polyomino_parameters_widget.hpp"
#include "direction.hpp"
#include "qpoint_hasher.hpp"
#include "polyomino_cell_item.hpp"
#include "polyomino_board_constants.hpp"
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
    width_              = parameters_widget_->width();
    height_             = parameters_widget_->height();
    max_polyomino_size_ = parameters_widget_->maxPolyominoSize();

    board_state_               = {};
    board_state_.mines         = parameters_widget_->minesCount();
    const size_t cells_counter = cells_.size();
    board_state_.empty_cells   = cells_counter - board_state_.mines;
    cells_.clear();

    std::uniform_int_distribution<size_t> size_distribution { 1, max_polyomino_size_ };
    std::vector<std::vector<size_t>>      matrix;
    matrix.resize(height_, std::vector(width_, constants::polyomino_board::empty_matrix_id));
    size_t id = 0;
    for (size_t row = 0; row < height_; ++row) {
        for (size_t col = 0; col < width_; ++col) {
            if (matrix[row][col] != constants::polyomino_board::empty_matrix_id) {
                continue;
            }

            auto cell                                  = std::make_unique<PolyominoCell>();
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
                const auto                                  neighbor_point = empty_neighbors[distribution(random_generator_)];
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

void PolyominoBoard::setupScene(BoardScene* scene)
{
    for (const auto& cell : cells_) {
        auto item = new PolyominoCellItem();
        item->initialize(cell.get(), generateCellColor());
        scene->registerCellItem(item);
    }

    scene->setSceneRect({ 0.,
                          0.,
                          static_cast<qreal>(width_ * SpriteCellItem::size()),
                          static_cast<qreal>(height_ * SpriteCellItem::size()) });
}

QWidget* PolyominoBoard::parametersWidget() const
{
    if (!parameters_widget_) {
        parameters_widget_ = new PolyominoParametersWidget { &dummy_parent_widget_ };
    }

    return parameters_widget_;
}

std::vector<size_t> PolyominoBoard::neighborIds(size_t id) const
{
    if (id < cells_.size()) {
        return cells_[id]->neighbor_ids;
    } else {
        Q_ASSERT(false && "Wrong id");
        return {};
    }
}

bool PolyominoBoard::isValidMatrixCoordinates(const QPoint& point, size_t width, size_t height) const
{
    return point.x() >= 0 && point.y() >= 0 && point.x() < width && point.y() < height;
}

void PolyominoBoard::setupNeighbors(const std::vector<std::vector<size_t>>& matrix, PolyominoCell& cell)
{
    const auto currentId = cell.id;
    for (const auto& shift : cell.shifts) {
        const auto currentSubCell = cell.center + shift;
        for (const auto direction : extended_directions_array) {
            const auto neighborCellCoords = currentSubCell + directionToShift(direction);
            if (!isValidMatrixCoordinates(neighborCellCoords, width_, height_)) {
                continue;
            }
            auto neighborId = matrix[neighborCellCoords.y()][neighborCellCoords.x()];
            if (neighborId == currentId || neighborId == constants::polyomino_board::empty_matrix_id) {
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

void PolyominoBoard::assignMines(size_t minesCount)
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

QColor PolyominoBoard::generateCellColor() const
{
    using namespace constants::polyomino_board;
    std::uniform_int_distribution<short> s(min_saturation, max_saturation);
    std::uniform_int_distribution<short> v(min_color_value, max_color_value);
    QColor                               color {QColor::Hsv};
    color.setHsv(hue, s(random_generator_), v(random_generator_));
    return color;
}

bool PolyominoBoard::isEmptyCell(const std::vector<std::vector<size_t>>& matrix, const QPoint& point) const
{
    return isValidMatrixCoordinates(point, matrix[0].size(), matrix.size())
           && matrix[point.y()][point.x()] == constants::polyomino_board::empty_matrix_id;
}

void PolyominoBoard::addEmptyNeighborCells(const std::vector<std::vector<size_t>>& matrix,
                                           const QPoint&                           point,
                                           std::deque<QPoint>&                     neighbors) const
{
    for (const auto direction : directions_array) {
        auto shift    = directionToShift(direction);
        auto neighbor = point + shift;
        if (isEmptyCell(matrix, neighbor)) {
            neighbors.push_back(neighbor);
        }
    }
}
