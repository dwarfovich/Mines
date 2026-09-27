#pragma once

#include "abstract_board.hpp"
#include "board.hpp"
#include "board_state.hpp"
#include "cell.hpp"

#include <queue>
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
    virtual void                reveal();
    virtual size_t              countNeighborMines(size_t id) const;
    virtual void                openAdjacentCells(Cell* cell);
    virtual void                initializeCells(size_t cells_counter);
    virtual void                randomize();

protected:  // data
    size_t                                 flags_ = 0;
    std::vector<std::unique_ptr<CellType>> cells_;
    mutable std::random_device             random_device_;
    mutable std::mt19937                   random_generator_;
};

template <typename CellType, typename ParametersWidgetType>
IdBasedBoard<CellType, ParametersWidgetType>::IdBasedBoard() : random_generator_{random_device_()}
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
        return cells_[id].get();
    } else {
        return nullptr;
    }
}

template <typename CellType, typename ParametersWidgetType>
void IdBasedBoard<CellType, ParametersWidgetType>::openCell(size_t id)
{
    auto cell = cellById(id);
    Q_ASSERT(cell);

    if (this->board_state_.game_state != GameState::Playing || cell->has_flag) {
        return;
    }

    if (!this->board_state_.first_cell_opened && cell->has_mine) {
        relocateFirstOpenedMine(cell);
    }
    this->board_state_.first_cell_opened = true;
    this->elapsed_time_ = std::chrono::steady_clock::now();

    if (cell->has_mine) {
        this->board_state_.game_state = GameState::Loose;
        reveal();
    } else {
        cell->is_closed = false;
        emit this->cellChanged(cell);
        --this->board_state_.empty_cells;
        cell->neighbor_mines = countNeighborMines(cell->id);
        if (this->board_state_.empty_cells == 0) {
            this->board_state_.game_state = GameState::Win;
            reveal();
        } else if (cell->neighbor_mines == 0) {
            openAdjacentCells(cell);
        }
    }
}

template <typename CellType, typename ParametersWidgetType>
void IdBasedBoard<CellType, ParametersWidgetType>::openAdjacentCells(Cell* cell)
{
    std::unordered_set<Cell*> checked_cells;
    std::queue<Cell*>         cells_to_open;
    cells_to_open.push(cell);
    while (!cells_to_open.empty()) {
        auto next_cell = cells_to_open.front();
        cells_to_open.pop();
        const auto [_, inserted] = checked_cells.insert(next_cell);
        if (!inserted) {
            continue;
        }
        if (!next_cell->has_mine && !next_cell->has_flag) {
            next_cell->is_closed = false;
            --this->board_state_.empty_cells;
            const auto mines = countNeighborMines(next_cell->id);
            if (mines == 0) {
                auto neighbors = neighborIds(next_cell->id);
                for (std::size_t i = 0; i < neighbors.size(); ++i) {
                    cells_to_open.push(cellById(neighbors[i]));
                }
            } else {
                next_cell->neighbor_mines = mines;
            }
            this->cellChanged(next_cell);
        }
    }
    // auto neighbors{neighborIds(cell->id)};
    // for (auto neighborId : neighbors) {
    //     auto neighborCell = cellById(neighborId);
    //     if (neighborCell->is_closed && !neighborCell->has_mine && !neighborCell->has_flag) {
    //         openCell(neighborId);
    //     }
    // }
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
        return cells_[id].get();
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
void IdBasedBoard<CellType, ParametersWidgetType>::reveal()
{
    for (size_t i = 0; i < cells_.size(); ++i) {
        if (cells_[i]->is_closed) {
            if (!cells_[i]->has_mine) {
                cells_[i]->neighbor_mines = countNeighborMines(i);
            } else if (this->board_state_.game_state == GameState::Win) {
                cells_[i]->has_flag = true;
            }
            cells_[i]->is_closed = false;
            this->cellChanged(cells_[i].get());
        }
    }
}

template <typename CellType, typename ParametersWidgetType>
size_t IdBasedBoard<CellType, ParametersWidgetType>::countNeighborMines(size_t id) const
{
    const auto& ids = neighborIds(id);
    size_t      mines = 0;
    for (const auto& neighborId : ids) {
        const auto* const cell = cellById(neighborId);
        if (cell->has_mine) {
            ++mines;
        }
    }

    return mines;
}

template <typename CellType, typename ParametersWidgetType>
void IdBasedBoard<CellType, ParametersWidgetType>::initializeCells(size_t cells_count)
{
    cells_.resize(cells_count);
    size_t mines_counter = 0;
    for (size_t i = 0; i < cells_.size(); ++i) {
        if (!cells_[i]) {
            cells_[i] = std::make_unique<CellType>();
        } else {
            *cells_[i] = {};
        }
        if (mines_counter < this->board_state_.mines) {
            cells_[i]->has_mine = true;
            ++mines_counter;
        } else {
            cells_[i]->has_mine = false;
        }
    }
}

template <typename CellType, typename ParametersWidgetType>
void IdBasedBoard<CellType, ParametersWidgetType>::randomize()
{
    std::shuffle(cells_.begin(), cells_.end(), random_generator_);
    for (size_t i = 0; i < cells_.size(); ++i) {
        cells_[i]->id = i;
    }
}