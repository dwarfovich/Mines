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

    std::size_t nodesCount() const;
    std::size_t minesCount() const;
    std::size_t maximumNeighbors();
    bool   disjointGraphAllowed();

private slots:
    void onNodesCountChanged(int count);

private:
    Ui::GraphParametersWidgetClass *ui_;
};
