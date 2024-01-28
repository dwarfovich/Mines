#pragma once

#include "id_based_board.hpp"
#include "nmino_cell.hpp"

#include <random>
#include <deque>

class NminoParametersWidget;

class NminoBoard : public IdBasedBoard<NminoCell>
{
public:
    NminoBoard();

    const QString&      id() const override;
    const QString&      name() const override;
    void                generate() override;
    void                setupScene(BoardScene* scene) override;
    QWidget*            parametersWidget() const override;
    std::vector<size_t> neighborIds(size_t id) const override;

    private:
    bool isValidMatrixCoordinates(const QPoint& point, size_t width, size_t height) const;
        void addShiftsFromCell(const NminoCell& cell, std::deque<QPoint>& queue) const;
    bool matrixHasEmptyNeighborSquares(const std::vector<std::vector<size_t>>& matrix,
                               const QPoint&                           center,
                               const std::deque<QPoint>&               cells) const;
        QColor randomColor() const;

private:
    static constexpr size_t        empty_matrix_id_ = 0;
    mutable NminoParametersWidget* parameters_widget_ = nullptr;
    std::random_device             random_device_;
    std::mt19937                   random_generator_;
};
