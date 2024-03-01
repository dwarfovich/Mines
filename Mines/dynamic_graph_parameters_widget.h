#pragma once

#include "ui_dynamic_graph_parameters_widget.h"

#include <QWidget>

class DynamicGraphParametersWidget : public QWidget
{
    Q_OBJECT

public:
    DynamicGraphParametersWidget(QWidget *parent = nullptr);

    std::size_t nodesCount() const;
    std::size_t minesCount() const;
    std::size_t maximumNeighbors() const;
    std::size_t speed() const;
    bool   allowDisjointGraph() const;

private slots:
    void onNodesCountChanged(int count);
    void onSpeedValueChanged(int speed);

private:
    Ui::DynamicGraphParametersWidgetClass ui_;
};
