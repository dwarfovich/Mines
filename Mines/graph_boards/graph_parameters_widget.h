#pragma once

#include "ui_graph_parameters_widget.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class GraphParametersWidgetClass;
};
QT_END_NAMESPACE

class GraphParametersWidget : public QWidget
{
    Q_OBJECT

public:
    GraphParametersWidget(QWidget *parent = nullptr);
    ~GraphParametersWidget();

    size_t minesCount() const { return 5; }
    size_t nodesCount() const { return 10; }
    size_t maximumNeighbors() { return 10; }
    bool   disjointGraphAllowed() { return false; }

private:
    Ui::GraphParametersWidgetClass *ui_;
};
