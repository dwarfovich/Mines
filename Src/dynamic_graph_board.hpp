#pragma once

#include "graph_board.hpp"
#include "dynamic_graph_parameters_widget.h"

class DynamicGraphBoard : public GraphBoard<DynamicGraphParametersWidget> {
public:
    const QString& id() const override;
    const QString& name() const override;
    void           generate() override;
    void           setupScene(BoardScene* scene) override;

private: // methods
    void setupParameters() override;
};
