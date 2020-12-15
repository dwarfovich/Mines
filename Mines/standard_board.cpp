#include "standard_board.hpp"
#include "gui/rectangle_board_parameters_widget.hpp"

#include <random>

#include <QDebug>
#define DEB qDebug()

int StandardBoard::flags() const
{
    return flags_;
}

const Cell *StandardBoard::cellById(int id) const
{
    if (id >= 0 && id < static_cast<int>(cells_.size())) {
        return &cells_[id];
    } else {
        return nullptr;
    }
}

void StandardBoard::openCell(int id)
{
    auto cell = cellById(id);
    Q_ASSERT(cell);

    if (board_state_.game_state != GameState::Playing || cell->hasFlag){
        return;
    }

    if (!board_state_.first_cell_opened && cell->hasMine){
        relocateMine(cell);
    }
    board_state_.first_cell_opened = true;

    if (cell->hasMine) {
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

void StandardBoard::toggleFlag(int id)
{
    auto cell = cellById(id);
    cell->hasFlag = !cell->hasFlag;
    if (cell->hasFlag) {
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
    auto widget = qobject_cast<RectangleBoardParametersWidget*>(parameters_widget);
    if (widget) {
        width_ = widget->boardWidth();
        height_ = widget->boardHeight();
        board_state_ = {};
        flags_ = 0;
        Q_ASSERT(width_ * height_ < size_t(std::numeric_limits<int>::max()));
        board_state_.mines = widget->mines();
        board_state_.empty_cells = width_ * height_ - board_state_.mines;
        board_state_.flags = board_state_.mines;

        initialize(width_ * height_);
        randomize();

        board_state_.game_state = GameState::Playing;
    }
}

Cell *StandardBoard::cellById(int id)
{
    DEB << "cells:" << cells_.size();
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
        if (cell->id != i && !cellById(i)->hasMine && cellById(i)->is_closed) {
            cell->hasMine = false;
            cellById(i)->hasMine = true;
            return;
        }
    }
}

void StandardBoard::reveal()
{
    for (size_t i = 0; i < cells_.size(); ++i) {
        if (cells_[i].is_closed) {
            if (!cells_[i].hasMine) {
                cells_[i].neighbor_mines = countNeighborMines(i);
                cellChanged(&cells_[i]);
            } else if (board_state_.game_state == GameState::Win) {
                cells_[i].hasFlag = true;
            }
            cells_[i].is_closed = false;
        }
    }
}

int StandardBoard::countNeighborMines(int id) const
{
    const auto& ids = neighborIds(id);
    int mines = 0;
    for (const auto& neighborId : ids) {
        const auto* cell = cellById(neighborId);
        if (cell->hasMine) {
            ++mines;
        }
    }

    return mines;
}

void StandardBoard::openAdjacentCells(Cell *cell)
{
    auto neighbors {neighborIds(cell->id)};
    for (auto neighborId : neighbors) {
        auto neighborCell = cellById( neighborId );
        if (neighborCell->is_closed && !neighborCell->hasMine&& !neighborCell->hasFlag){
            openCell(neighborId);
        }
    }
}

void StandardBoard::initialize(size_t cells_counter)
{
    cells_.resize(cells_counter);
    int mines_counter = 0;
    for (size_t i = 0; i < cells_.size(); ++i) {
        cells_[i] = {};
        if (mines_counter < board_state_.mines) {
            cells_[i].hasMine = true;
            ++mines_counter;
        }
    }
}

void StandardBoard::randomize()
{
    std::random_device random_device;
    std::mt19937 generator {random_device()};
    std::shuffle(cells_.begin(), cells_.end(), generator);
    for (size_t i = 0; i < cells_.size(); ++i) {
        cells_[i].id = i;
    }
}
