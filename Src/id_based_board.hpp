#pragma once

#include "abstract_board.hpp"
#include "board.hpp"
#include "board_state.hpp"
#include "cell.hpp"
#include "rectangle_board_parameters_widget.hpp"

#include <random>

template <typename CellType = Cell, typename ParametersWidgetType = RectangleBoardParametersWidget>
class IdBasedBoard : public AbstractBoard<ParametersWidgetType> {
public:
    IdBasedBoard();

    size_t          flags() const override;
    const CellType* cellById(size_t id) const override;
    void            openCell(size_t id) override;
    void            toggleFlag(size_t id) override;

protected:  // methods
    virtual std::vector<size_t> neighborIds(size_t id) const = 0;
    virtual CellType*           cellById(size_t id);
    virtual void                relocateFirstOpenedMine(Cell* cell);
    virtual void                revealField();
    virtual std::size_t         countNeighborMines(size_t id) const;
    virtual std::size_t         countNeighborMines(const std::vector<std::size_t>& neighbors) const;
    virtual void                revealCell(Cell* cell);
    virtual void                revealCells(std::vector<std::size_t> ids);
    virtual void                initializeCells(size_t cells_counter);
    virtual void                randomize();

protected:  // data
    using AbstractBoard<ParametersWidgetType>::board_state_;

    size_t                     flags_ = 0;
    std::vector<CellType>      cells_;
    mutable std::random_device random_device_;
    mutable std::mt19937       random_generator_;
};

template <typename CellType, typename ParametersWidgetType>
// IdBasedBoard<CellType, ParametersWidgetType>::IdBasedBoard() : random_generator_{random_device_()}
IdBasedBoard<CellType, ParametersWidgetType>::IdBasedBoard() : random_generator_{0}
{
}

template <typename CellType, typename ParametersWidgetType>
size_t IdBasedBoard<CellType, ParametersWidgetType>::flags() const
{
    return flags_;
}

template <typename CellType, typename ParametersWidgetType>
const CellType* IdBasedBoard<CellType, ParametersWidgetType>::cellById(size_t id) const
{
    if (id >= 0 && id < cells_.size()) {
        return &cells_[id];
    } else {
        return nullptr;
    }
}

template <typename CellType, typename ParametersWidgetType>
void IdBasedBoard<CellType, ParametersWidgetType>::openCell(size_t id)
{
    auto cell = cellById(id);
    Q_ASSERT(cell);

    if (board_state_.game_state != GameState::Playing || cell->has_flag) {
        return;
    }
    if (!board_state_.first_cell_opened && cell->has_mine) {
        relocateFirstOpenedMine(cell);
    }
    if (!board_state_.first_cell_opened) {
        board_state_.first_cell_opened = true;
        this->elapsed_time_ = std::chrono::steady_clock::now();
        this->base_timer_.start();
    }
    if (cell->has_mine) {
        board_state_.game_state = GameState::Loose;
        this->base_timer_.stop();
        this->stopGame();
        revealField();
    } else {
        revealCell(cell);
    }
}

template <typename CellType, typename ParametersWidgetType>
void IdBasedBoard<CellType, ParametersWidgetType>::revealCell(Cell* cell)
{
    if (!cell->is_closed) {
        return;
    }

    cell->is_closed = false;
    board_state_.decreaseEmptyCells(1);
    auto neighbor_ids = neighborIds(cell->id);
    cell->neighbor_mines = countNeighborMines(neighbor_ids);
    emit this->cellChanged(cell);
    if (cell->neighbor_mines == 0) {
        revealCells(neighbor_ids);
    }
    if (board_state_.empty_cells == 0) {
        board_state_.game_state = GameState::Win;
        this->base_timer_.stop();
        this->stopGame();
        revealField();
    }
}

template <typename CellType, typename ParametersWidgetType>
void IdBasedBoard<CellType, ParametersWidgetType>::revealCells(std::vector<size_t> ids)
{
    std::unordered_set<std::size_t> checked_cells;
    while (!ids.empty()) {
        const auto cell_id = ids.back();
        ids.pop_back();
        const auto [_, inserted] = checked_cells.insert(cell_id);
        if (!inserted) {
            continue;
        }
        auto* cell = cellById(cell_id);
        if (cell->is_closed && !cell->has_mine && !cell->has_flag) {
            cell->is_closed = false;
            board_state_.decreaseEmptyCells(1);
            const auto neighbors = neighborIds(cell_id);
            const auto mines = countNeighborMines(neighbors);
            if (mines == 0) {
                ids.append_range(neighbors);
            } else {
                cell->neighbor_mines = mines;
            }
            this->cellChanged(cell);
        }
    }
}

template <typename CellType, typename ParametersWidgetType>
void IdBasedBoard<CellType, ParametersWidgetType>::toggleFlag(size_t id)
{
    auto cell = cellById(id);
    if (cell->is_closed) {
        cell->has_flag = !cell->has_flag;
        if (cell->has_flag) {
            ++flags_;
        } else {
            --flags_;
        }
        this->cellChanged(cell);
    }
}

template <typename CellType, typename ParametersWidgetType>
CellType* IdBasedBoard<CellType, ParametersWidgetType>::cellById(size_t id)
{
    if (id >= 0 && id < static_cast<int>(cells_.size())) {
        return &cells_[id];
    } else {
        return nullptr;
    }
}

template <typename CellType, typename ParametersWidgetType>
void IdBasedBoard<CellType, ParametersWidgetType>::relocateFirstOpenedMine(Cell* cell)
{
    Q_ASSERT(cell);

    for (int i = 0; i < static_cast<int>(cells_.size()); ++i) {
        if (cell->id != i && !cellById(i)->has_mine && cellById(i)->is_closed) {
            cell->has_mine = false;
            cellById(i)->has_mine = true;
            return;
        }
    }
}

template <typename CellType, typename ParametersWidgetType>
void IdBasedBoard<CellType, ParametersWidgetType>::revealField()
{
    for (size_t i = 0; i < cells_.size(); ++i) {
        if (cells_[i].is_closed) {
            if (!cells_[i].has_mine) {
                cells_[i].neighbor_mines = countNeighborMines(i);
            } else if (this->board_state_.game_state == GameState::Win) {
                cells_[i].has_flag = true;
            }
            cells_[i].is_closed = false;
            this->cellChanged(&cells_[i]);
        }
    }
}

template <typename CellType, typename ParametersWidgetType>
size_t IdBasedBoard<CellType, ParametersWidgetType>::countNeighborMines(size_t id) const
{
    const auto& ids = neighborIds(id);

    return countNeighborMines(ids);
}

template <typename CellType, typename ParametersWidgetType>
size_t IdBasedBoard<CellType, ParametersWidgetType>::countNeighborMines(const std::vector<std::size_t>& neighbors) const
{
    const size_t mines = std::accumulate(
        neighbors.cbegin(),
        neighbors.cend(),
        std::size_t{0},
        [this](std::size_t sum, std::size_t id) { return sum + cellById(id)->has_mine; });

    return mines;
}

template <typename CellType, typename ParametersWidgetType>
void IdBasedBoard<CellType, ParametersWidgetType>::initializeCells(size_t cells_count)
{
    cells_.resize(cells_count);
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

template <typename CellType, typename ParametersWidgetType>
void IdBasedBoard<CellType, ParametersWidgetType>::randomize()
{
    std::shuffle(cells_.begin(), cells_.end(), random_generator_);
    for (size_t i = 0; i < cells_.size(); ++i) {
        cells_[i].id = i;
    }
}