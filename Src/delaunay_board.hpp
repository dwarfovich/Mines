#pragma once

#include "delaunay_parameters_widget.hpp"
#include "graph_board.hpp"
#include "triangulator.hpp"

class DelaunayBoard : public GraphBoard<DelaunayParametersWidget> {
    Q_OBJECT

public:
    const QString& id() const override;
    const QString& name() const override;
    void           generate() override;

protected:  // methods
    std::vector<std::size_t> neighborIds(std::size_t id) const override;
    void                     formNeighbors(const Triangulator& triangulator);
    void                     setupParameters() override;

protected:  // data
    Triangulator triangulator_;
};