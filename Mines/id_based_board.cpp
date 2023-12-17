#include "id_based_board.hpp"
#include "gui/rectangle_board_parameters_widget.hpp"

#include <random>

size_t IdBasedBoard::flags() const
{
    return flags_;
}

const Cell *IdBasedBoard::cellById(size_t id) const
{
    if (id >= 0 && id < cells_.size()) {
        return &cells_[id];
    } else {
        return nullptr;
    }
}

void IdBasedBoard::openCell(size_t id)
{
    auto cell = cellById(id);
    Q_ASSERT(cell);

    if (board_state_.game_state != GameState::Playing || cell->has_flag) {
        return;
    }

    if (!board_state_.first_cell_opened && cell->has_mine) {
        relocateFirstOpenedMine(cell);
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

void IdBasedBoard::toggleFlag(size_t id)
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

Cell *IdBasedBoard::cellById(size_t id)
{
    if (id >= 0 && id < static_cast<int>(cells_.size())) {
        return &cells_[id];
    } else {
        return nullptr;
    }
}

void IdBasedBoard::relocateFirstOpenedMine(Cell *cell)
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

void IdBasedBoard::reveal()
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

size_t IdBasedBoard::countNeighborMines(size_t id) const
{
    const auto &ids   = neighborIds(id);
    size_t      mines = 0;
    for (const auto &neighborId : ids) {
        const auto *const cell = cellById(neighborId);
        if (cell->has_mine) {
            ++mines;
        }
    }

    return mines;
}

void IdBasedBoard::openAdjacentCells(Cell *cell)
{
    auto neighbors { neighborIds(cell->id) };
    for (auto neighborId : neighbors) {
        auto neighborCell = cellById(neighborId);
        if (neighborCell->is_closed && !neighborCell->has_mine && !neighborCell->has_flag) {
            openCell(neighborId);
        }
    }
}

void IdBasedBoard::initializeCells(size_t cells_counter)
{
    cells_.resize(cells_counter);
    size_t mines_counter = 0;
    for (size_t i = 0; i < cells_.size(); ++i) {
        cells_[i] = {};
        if (mines_counter < board_state_.mines) {
            cells_[i].has_mine = true;
            ++mines_counter;
        } else {
            cells_[i].has_mine = false;
        }
    }
}

void IdBasedBoard::randomize()
{
    std::random_device random_device;
    std::mt19937       generator { random_device() };
    std::shuffle(cells_.begin(), cells_.end(), generator);
    for (size_t i = 0; i < cells_.size(); ++i) {
        cells_[i].id = i;
    }
}
