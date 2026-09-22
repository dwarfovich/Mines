#pragma once

#include "id_based_board.hpp"
#include "polyomino_cell.hpp"
#include "polyomino_parameters_widget.hpp"

#include <deque>
#include <random>

class PolyominoBoard : public IdBasedBoard<PolyominoCell, PolyominoParametersWidget> {
    Q_OBJECT
public:
    const QString&      id() const override;
    const QString&      name() const override;
    void                generate() override;
    void                setupScene(BoardScene* scene) override;
    std::vector<size_t> neighborIds(size_t id) const override;

private:
    bool   isValidMatrixCoordinates(const QPoint& point, size_t width, size_t height) const;
    void   setupNeighbors(const std::vector<std::vector<size_t>>& matrix, PolyominoCell& cell);
    void   assignMines(size_t minesCount);
    QColor generateCellColor() const;
    bool   isEmptyCell(const std::vector<std::vector<size_t>>& matrix, const QPoint& point) const;
    bool   addEmptyNeighborCells(const std::vector<std::vector<size_t>>& matrix,
                                 const QPoint&                           point,
                                 std::deque<QPoint>&                     neighbors) const;

private:
    size_t                             width_ = 0;
    size_t                             height_ = 0;
    size_t                             max_polyomino_size_ = 1;
};
