#ifndef ID_BASED_BOARD_HPP
#define ID_BASED_BOARD_HPP

#include "board.hpp"
#include "cell.hpp"

#include <random>

template<typename CellType>
class IdBasedBoard : public Board
{
public:
    size_t      flags() const override;
    const Cell *cellById(size_t id) const override;
    void        openCell(size_t id) override;
    void        toggleFlag(size_t id) override;

protected: // methods
    virtual std::vector<size_t> neighborIds(size_t id) const = 0;
    virtual Cell               *cellById(size_t id);
    virtual void                relocateFirstOpenedMine(Cell *cell);
    virtual void                reveal();
    virtual size_t              countNeighborMines(size_t id) const;
    virtual void                openAdjacentCells(Cell *cell);
    virtual void                initializeCells(size_t cells_counter);
    virtual void                randomize();

protected: // data
    size_t                                 flags_ = 0;
    std::vector<std::unique_ptr<CellType>> cells_;
};

template<typename CellType>
size_t IdBasedBoard<CellType>::flags() const
{
    return flags_;
}

template<typename CellType>
const Cell *IdBasedBoard<CellType>::cellById(size_t id) const
{
    if (id >= 0 && id < cells_.size()) {
        return cells_[id].get();
    } else {
        return nullptr;
    }
}

template<typename CellType>
void IdBasedBoard<CellType>::openCell(size_t id)
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

template<typename CellType>
void IdBasedBoard<CellType>::toggleFlag(size_t id)
{
    auto cell = cellById(id);
    if (cell->is_closed) {
        cell->has_flag = !cell->has_flag;
        if (cell->has_flag) {
            ++flags_;
        } else {
            --flags_;
        }
        cellChanged(cell);
    }
}

template<typename CellType>
Cell *IdBasedBoard<CellType>::cellById(size_t id)
{
    if (id >= 0 && id < static_cast<int>(cells_.size())) {
        return cells_[id].get();
    } else {
        return nullptr;
    }
}

template<typename CellType>
void IdBasedBoard<CellType>::relocateFirstOpenedMine(Cell *cell)
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

template<typename CellType>
void IdBasedBoard<CellType>::reveal()
{
    for (size_t i = 0; i < cells_.size(); ++i) {
        if (cells_[i]->is_closed) {
            if (!cells_[i]->has_mine) {
                cells_[i]->neighbor_mines = countNeighborMines(i);
            } else if (board_state_.game_state == GameState::Win) {
                cells_[i]->has_flag = true;
            }
            cells_[i]->is_closed = false;
            cellChanged(cells_[i].get());
        }
    }
}

template<typename CellType>
size_t IdBasedBoard<CellType>::countNeighborMines(size_t id) const
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

template<typename CellType>
void IdBasedBoard<CellType>::openAdjacentCells(Cell *cell)
{
    auto neighbors { neighborIds(cell->id) };
    for (auto neighborId : neighbors) {
        auto neighborCell = cellById(neighborId);
        if (neighborCell->is_closed && !neighborCell->has_mine && !neighborCell->has_flag) {
            openCell(neighborId);
        }
    }
}

template<typename CellType>
void IdBasedBoard<CellType>::initializeCells(size_t cells_counter)
{
    cells_.resize(cells_counter);
    size_t mines_counter = 0;
    for (size_t i = 0; i < cells_.size(); ++i) {
        if (!cells_[i]) {
            cells_[i] = std::make_unique<CellType>();
        }
        if (mines_counter < board_state_.mines) {
            cells_[i]->has_mine = true;
            ++mines_counter;
        } else {
            cells_[i]->has_mine = false;
        }
    }
}

template<typename CellType>
void IdBasedBoard<CellType>::randomize()
{
    std::random_device random_device;
    std::mt19937       generator { random_device() };
    std::shuffle(cells_.begin(), cells_.end(), generator);
    for (size_t i = 0; i < cells_.size(); ++i) {
        cells_[i]->id = i;
    }
}

#endif // ID_BASED_BOARD_HPP
