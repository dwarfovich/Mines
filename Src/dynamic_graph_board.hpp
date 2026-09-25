#pragma once

#include "dynamic_graph_parameters_widget.hpp"
#include "graph_board.hpp"

class DynamicGraphBoard : public GraphBoard<DynamicGraphParametersWidget> {
public:
    const QString& id() const override;
    const QString& name() const override;
    void           generate() override;
    void           setupScene(BoardScene* scene) override;

private:  // methods
    void setupParameters() override;

private:  // data
    inline static constexpr double user_speed_conversion_coefficient = 2. / 40.;
    inline static constexpr double random_angle_range = 0.4;
};
