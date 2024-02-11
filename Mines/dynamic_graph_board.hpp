#pragma once

#include "graph_board.hpp"

class DynamicGraphBoard : public GraphBoard
{
public:
    const QString &id() const override;
    const QString &name() const override;
    void           setupScene(BoardScene *scene) override;

private:
    qreal speed_ = 0.;
};
