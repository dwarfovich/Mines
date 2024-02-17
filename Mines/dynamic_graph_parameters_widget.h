#pragma once

#include "ui_dynamic_graph_parameters_widget.h"

#include <QWidget>

class DynamicGraphParametersWidget : public QWidget
{
    Q_OBJECT

public:
    DynamicGraphParametersWidget(QWidget *parent = nullptr);

    size_t nodesCount() const;
    size_t minesCount() const;
    size_t maximumNeighbors() const;
    size_t speed() const;
    bool   allowDisjointGraph() const;

private slots:
    void onNodesCountChanged(int count);
    void onSpeedValueChanged(int speed);

private:
    Ui::DynamicGraphParametersWidgetClass ui_;
};
