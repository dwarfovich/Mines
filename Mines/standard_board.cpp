#include "standard_board.hpp"
#include "gui/rectangle_board_parameters_widget.hpp"

#include <random>

#include <QDebug>
#define DEB qDebug()

size_t StandardBoard::flags() const
{
    return flags_;
}

const Cell *StandardBoard::cellById(size_t id) const
{
    if (id >= 0 && id < cells_.size()) {
        return &cells_[id];
    } else {
        return nullptr;
    }
}

void StandardBoard::openCell(size_t id)
{
    auto cell = cellById(id);
    Q_ASSERT(cell);

    if (board_state_.game_state != GameState::Playing || cell->has_flag) {
        return;
    }

    if (!board_state_.first_cell_opened && cell->has_mine) {
        relocateMine(cell);
    }
    board_state_.first_cell_opened = true;

    if (cell->has_mine) {
        board_state_.game_state = GameState::Loose;
        reveal();
    } else {
        cell->is_closed = false;
        emit cellChanged(cell);
        --board_state_.empty_cells;
        cell->neighbor_mines = countNeighborMines(cell->id);
        if (board_state_.empty_cells == 0) {
            board_state_.game_state = GameState::Win;
            reveal();
        } else if (cell->neighbor_mines == 0) {
            openAdjacentCells(cell);
        }
    }
}

void StandardBoard::toggleFlag(size_t id)
{
    auto cell      = cellById(id);
    cell->has_flag = !cell->has_flag;
    if (cell->has_flag) {
        ++flags_;
    } else {
        --flags_;
    }
    cellChanged(cell);
}

const BoardState &StandardBoard::boardState() const
{
    return board_state_;
}

QWidget *StandardBoard::createParametersWidget() const
{
    return new RectangleBoardParametersWidget {};
}

void StandardBoard::generate(QWidget *parameters_widget)
{
    auto widget = qobject_cast<RectangleBoardParametersWidget *>(parameters_widget);
    if (widget) {
        width_                   = widget->boardWidth();
        height_                  = widget->boardHeight();
        board_state_             = {};
        flags_                   = 0;
        board_state_.mines       = widget->mines();
        board_state_.empty_cells = width_ * height_ - board_state_.mines;

        initialize(width_ * height_);
        randomize();

        board_state_.game_state = GameState::Playing;
    }
}

Cell *StandardBoard::cellById(size_t id)
{
    if (id >= 0 && id < static_cast<int>(cells_.size())) {
        return &cells_[id];
    } else {
        return nullptr;
    }
}

void StandardBoard::relocateMine(Cell *cell)
{
    Q_ASSERT(cell);

    for (int i = 0; i < static_cast<int>(cells_.size()); ++i) {
        if (cell->id != i && !cellById(i)->has_mine && cellById(i)->is_closed) {
            cell->has_mine        = false;
            cellById(i)->has_mine = true;
            return;
        }
    }
}

void StandardBoard::reveal()
{
    for (size_t i = 0; i < cells_.size(); ++i) {
        if (cells_[i].is_closed) {
            if (!cells_[i].has_mine) {
                cells_[i].neighbor_mines = countNeighborMines(i);
                cellChanged(&cells_[i]);
            } else if (board_state_.game_state == GameState::Win) {
                cells_[i].has_flag = true;
            }
            cells_[i].is_closed = false;
        }
    }
}

size_t StandardBoard::countNeighborMines(size_t id) const
{
    const auto &ids   = neighborIds(id);
    int         mines = 0;
    for (const auto &neighborId : ids) {
        const auto *cell = cellById(neighborId);
        if (cell->has_mine) {
            ++mines;
        }
    }

    return mines;
}

void StandardBoard::openAdjacentCells(Cell *cell)
{
    auto neighbors { neighborIds(cell->id) };
    for (auto neighborId : neighbors) {
        auto neighborCell = cellById(neighborId);
        if (neighborCell->is_closed && !neighborCell->has_mine && !neighborCell->has_flag) {
            openCell(neighborId);
        }
    }
}

void StandardBoard::initialize(size_t cells_counter)
{
    cells_.resize(cells_counter);
    size_t mines_counter = 0;
    for (size_t i = 0; i < cells_.size(); ++i) {
        cells_[i] = {};
        if (mines_counter < board_state_.mines) {
            cells_[i].has_mine = true;
            ++mines_counter;
        }
    }
}

void StandardBoard::randomize()
{
    std::random_device random_device;
    std::mt19937       generator { random_device() };
    std::shuffle(cells_.begin(), cells_.end(), generator);
    for (size_t i = 0; i < cells_.size(); ++i) {
        cells_[i].id = i;
    }
}
