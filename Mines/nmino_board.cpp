#include "nmino_board.hpp"
#include "nmino_parameters_widget.h"
#include "direction.hpp"

#include <unordered_set>
#include <queue>

NminoBoard::NminoBoard() : random_generator_ { random_device_() }
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
    const size_t width          = 10;
    const size_t height         = 10;
    const size_t max_nmino_size = 5;

    std::uniform_int_distribution<size_t> distribution(1, max_nmino_size);
    std::uniform_int_distribution<unsigned short> direction_distribution(0, 3);

    cells_.clear();
    std::unordered_map<size_t, std::unordered_set<size_t>> neighbors;

    static constexpr size_t          empty_id = -1;
    std::vector<std::vector<size_t>> matrix;
    matrix.resize(width, std::vector(height, empty_id));

    for (size_t col = 0; col < width; ++col) {
        for (size_t row = 0; row < height; ++row) {
            if (matrix[col][row] != empty_id) {
                continue;
            }

            auto cell = std::make_unique<NminoCell>();
            cell->row = row;
            cell->col = col;


            const auto target_size = distribution(random_generator_);
            size_t     current_size = 1;
            std::queue<QPoint> queue;
            queue.push({ row, col });
            while (current_size < target_size) {
                do {
                    auto direction = Direction(direction_distribution(random_generator_));
                    //auto new_coords = 
                } while (true);
            }


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
