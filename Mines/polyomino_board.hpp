#pragma once

#include "id_based_board.hpp"
#include "polyomino_cell.hpp"

#include <random>
#include <deque>

class PolyominoParametersWidget;

class PolyominoBoard : public IdBasedBoard<PolyominoCell>
{
public:
    PolyominoBoard();

    const QString&      id() const override;
    const QString&      name() const override;
    void                generate() override;
    void                setupScene(BoardScene* scene) override;
    QWidget*            parametersWidget() const override;
    std::vector<size_t> neighborIds(size_t id) const override;

private:
    bool   isValidMatrixCoordinates(const QPoint& point, size_t width, size_t height) const;
    void   setupNeighbors(const std::vector<std::vector<size_t>>& matrix, PolyominoCell& cell);
    void   assignMines(size_t minesCount);
    QColor generateCellColor() const;
    bool   isEmptyCell(const std::vector<std::vector<size_t>>& matrix, const QPoint& point) const;
    void   addEmptyNeighborCells(const std::vector<std::vector<size_t>>& matrix,
                                 const QPoint&                           point,
                                 std::deque<QPoint>&                     neighbors) const;

private:

    size_t                             width_              = 0;
    size_t                             height_             = 0;
    size_t                             max_polyomino_size_ = 1;
    mutable PolyominoParametersWidget* parameters_widget_  = nullptr;
    mutable std::random_device         random_device_;
    mutable std::mt19937               random_generator_;
};
