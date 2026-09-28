#pragma once

#include "delaunay_parameters_widget.hpp"
#include "graph_board.hpp"
#include "graph_cell.hpp"
#include "triangulator.hpp"

class DelaunayBoard : public GraphBoard<GraphCell, DelaunayParametersWidget> {
    Q_OBJECT

public:
    const QString& id() const override;
    const QString& name() const override;
    void           generate() override;

protected:  // methods
    void formNeighbors(const Triangulator& triangulator);
    void setupParameters() override;

protected:  // data
    Triangulator triangulator_;
};