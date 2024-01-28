#include "nmino_board.hpp"
#include "nmino_parameters_widget.h"
#include "direction.hpp"

#include <unordered_set>
#include <queue>

#include <iostream>

// NminoBoard::NminoBoard() : random_generator_ { random_device_() }
NminoBoard::NminoBoard() : random_generator_ { 1 }
{
}

const QString& NminoBoard::id() const
{
    static const QString id = "Nmino";
    return id;
}

const QString& NminoBoard::name() const
{
    static const QString name = "Nmino";
    return name;
}

void NminoBoard::generate()
{
    const size_t width          = 5;
    const size_t height         = 5;
    const size_t max_nmino_size = 5;

    std::uniform_int_distribution<size_t>         distribution(1, max_nmino_size);
    std::uniform_int_distribution<unsigned short> direction_distribution(0, 3);
    std::uniform_int_distribution<unsigned short> use_square_distribution(0, 1);

    cells_.clear();
    std::unordered_map<size_t, std::unordered_set<size_t>> neighbors;

    
    std::vector<std::vector<size_t>> matrix;
    matrix.resize(width, std::vector(height, empty_matrix_id_));
    size_t id = 1;
    for (size_t row = 0; row < height; ++row) {
        for (size_t col = 0; col < width; ++col) {
            if (matrix[col][row] != empty_matrix_id_) {
                continue;
            }

            auto cell    = std::make_unique<NminoCell>();
            cell->id     = id;
            cell->center = { static_cast<int>(col), static_cast<int>(row) };
            matrix[cell->center.x()][cell->center.y()] = id;

            const auto         target_size  = distribution(random_generator_);
            size_t             current_size = 1;
            std::deque<QPoint> shifts;
            shifts.push_back({ 0, 0 });
            do {
                if (shifts.empty()) {
                    addShiftsFromCell(*cell, shifts);
                }
                if (!matrixHasEmptyNeighborSquares(matrix, cell->center, shifts)) {
                    break;
                }
                
                auto current_shift = shifts.front();
                shifts.pop_front();
                auto current_point = cell->center + current_shift;
                if (shifts.size() != 0 && !use_square_distribution(random_generator_)) {
                    continue;
                }

                auto direction = Direction(direction_distribution(random_generator_));
                bool found     = false;
                for (int i = 0; !found && i < 4; ++i) {
                    auto new_shift = current_shift + directionToShift(direction);
                    auto new_point = cell->center + new_shift;
                    if (isValidMatrixCoordinates(new_point, width, height)
                        && matrix[new_point.x()][new_point.y()] == empty_matrix_id_) {
                        matrix[new_point.x()][new_point.y()] = id;
                        ++current_size;
                        found = true;
                        cell->shifts.push_back(new_shift);
                    } else {
                        direction = nextDirection(direction);
                    }
                }
            } while (current_size <= target_size);

            ++id;
        }
    }
}

void NminoBoard::setupScene(BoardScene* scene)
{
}

QWidget* NminoBoard::parametersWidget() const
{
    if (!parameters_widget_) {
        parameters_widget_ = new NminoParametersWidget { &dummy_parent_widget_ };
    }

    return parameters_widget_;
}

std::vector<size_t> NminoBoard::neighborIds(size_t id) const
{
    return std::vector<size_t>();
}

bool NminoBoard::isValidMatrixCoordinates(const QPoint& point, size_t width, size_t height) const
{
    return point.x() >= 0 && point.y() >= 0 && point.x() < width && point.y() < height;
}

void NminoBoard::addShiftsFromCell(const NminoCell& cell, std::deque<QPoint>& queue) const
{
    for (const auto& point : cell.shifts) {
        queue.push_back(point);
    }
}

bool NminoBoard::matrixHasEmptyNeighborSquares(const std::vector<std::vector<size_t>>& matrix,
                                               const QPoint&                           center,
                                               const std::deque<QPoint>&               shifts) const
{
    for (const auto& shift : shifts) {
        for (uint8_t i = 0; i < 4; ++i) {
            auto neighbor_shift = directionToShift(Direction(i));
            auto point          = center + shift + neighbor_shift;
            if (isValidMatrixCoordinates(point, matrix.size(), matrix[0].size())
                && matrix[point.x()][point.y()] == empty_matrix_id_) {
                return true;
            }
        }
    }
    return false;
}

QColor NminoBoard::randomColor() const
{
    std::random_device random_device;
    // std::mt19937                         random_generator(random_device());
    std::mt19937                         random_generator(1);
    std::uniform_int_distribution<short> color(0, 255);

    return { color(random_generator), color(random_generator), color(random_generator) };
}
