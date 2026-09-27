#pragma once

#include "dynamic_graph_cell.hpp"
#include "dynamic_graph_parameters_widget.hpp"
#include "graph_board.hpp"

class DynamicGraphBoard : public GraphBoard<DynamicGraphCell, DynamicGraphParametersWidget> {
public:
    DynamicGraphBoard();

    const QString& id() const override;
    const QString& name() const override;
    void           generate() override;
    void           setupScene(BoardScene* scene) override;
    void           startGame() override;
    void           stopGame() override;

private:  // methods
    void setupParameters() override;
    void advanceCells();

private:  // data
    inline static constexpr qreal user_speed_conversion_coefficient = 0.5;
    inline static constexpr qreal random_angle_range = 0.4;
    inline static constexpr qreal critical_radius_coefficient = 0.9;
    inline static constexpr int   advance_period = 1000 / 60;

    QTimer timer_;
};
