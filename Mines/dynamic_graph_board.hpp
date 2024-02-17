#pragma once

#include "graph_board.hpp"

class DynamicGraphParametersWidget;

class DynamicGraphBoard : public GraphBoard
{
public:
    const QString &id() const override;
    const QString &name() const override;
    void           generate() override;
    void           setupScene(BoardScene *scene) override;
    QWidget       *parametersWidget() const;

private:
    void setupParameters() override;

private:
    mutable DynamicGraphParametersWidget *parameters_widget_ = nullptr;
};
