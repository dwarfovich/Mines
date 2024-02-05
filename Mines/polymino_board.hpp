#pragma once

#include "id_based_board.hpp"
#include "polymino_cell.hpp"

#include <random>
#include <deque>

class PolyminoParametersWidget;

class PolyminoBoard : public IdBasedBoard<PolyminoCell>
{
public:
    PolyminoBoard();

    const QString&      id() const override;
    const QString&      name() const override;
    void                generate() override;
    void                setupScene(BoardScene* scene) override;
    QWidget*            parametersWidget() const override;
    std::vector<size_t> neighborIds(size_t id) const override;

private:
    bool   isValidMatrixCoordinates(const QPoint& point, size_t width, size_t height) const;
    QColor generateRandomColor() const;
    bool   isEmptyCell(const std::vector<std::vector<size_t>>& matrix, const QPoint& point) const;
    void   addEmptyNeighborCells(const std::vector<std::vector<size_t>>& matrix,
                                 const QPoint&                           point,
                                 std::deque<QPoint>&                     neighbors) const;

private:
    static constexpr size_t           empty_matrix_id_   = 0;
    static constexpr int              cell_square_size_  = 50;
    size_t                            width_             = 0;
    size_t                            height_            = 0;
    mutable PolyminoParametersWidget* parameters_widget_ = nullptr;
    std::random_device                random_device_;
    std::mt19937                      random_generator_;
};
