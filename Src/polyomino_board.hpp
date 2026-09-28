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

private:  // methods
    void setupBoard();
    bool isValidMatrixCoordinates(const QPoint& point, size_t width, size_t height) const;
    void setupNeighbors(const std::vector<std::vector<size_t>>& matrix, PolyominoCell& cell);
    void setupMines();
    bool isEmptyCell(const std::vector<std::vector<size_t>>& matrix, const QPoint& point) const;
    void addEmptyNeighborCells(const std::vector<std::vector<size_t>>& matrix,
                               const QPoint&                           point,
                               std::vector<QPoint>&                    neighbors) const;
    using IdsMatrix = std::vector<std::vector<size_t>>;
    void setupNeighbors(const IdsMatrix& ids);
    void generatePolyomino(PolyominoCell& cell, IdsMatrix& ids);

private:  // data
    size_t width_ = 0;
    size_t height_ = 0;
    size_t max_polyomino_size_ = 1;
};
