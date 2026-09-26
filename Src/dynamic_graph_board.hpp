#pragma once

#include "dynamic_graph_cell.hpp"
#include "dynamic_graph_parameters_widget.hpp"
#include "graph_board.hpp"

class DynamicGraphBoard : public GraphBoard<DynamicGraphCell, DynamicGraphParametersWidget> {
public:
    DynamicGraphBoard()
    {
        connect(&timer_, &QTimer::timeout, [this]() { advanceCells(); });
    }

    const QString& id() const override;
    const QString& name() const override;
    void           generate() override;
    void           setupScene(BoardScene* scene) override;
    void           startGame() override
    {
        timer_.start(33);
    }

private:  // methods
    void setupParameters() override;
    void advanceCells()
    {
        for (auto& cell : cells_) {
            static constexpr qreal critical_radius = 300;
            const auto&            field = boundingRect();
            const qreal            field_radius = std::hypot(field.width(), field.height()) / 2.0;
            const auto             center = field.center();
            const QLineF           lineToCenter = {QPointF{cell->x(), cell->y()}, center};
            if (lineToCenter.length() >= field_radius * 0.90) {
                cell->angle = std::atan2(-lineToCenter.dy(), lineToCenter.dx());
            } else {
                cell->angle += QRandomGenerator::global()->bounded(random_angle_range) - random_angle_range / 2.;
            }
            cell->coordinates.setX(cell->x() + cell->speed * std::cos(cell->angle));
            cell->coordinates.setY(cell->y() - cell->speed * std::sin(cell->angle));
        }
    }

private:  // data
    inline static constexpr double user_speed_conversion_coefficient = 0.5;
    inline static constexpr double random_angle_range = 0.4;

    QTimer timer_;
};
