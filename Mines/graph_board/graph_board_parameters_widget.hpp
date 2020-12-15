#ifndef GRAPH_BOARD_PARAMETERS_WIDGET_HPP
#define GRAPH_BOARD_PARAMETERS_WIDGET_HPP

#include <QWidget>

namespace Ui {
class GraphBoardParametersWidget;
}

class GraphBoardParametersWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GraphBoardParametersWidget(QWidget *parent = nullptr);
    ~GraphBoardParametersWidget();

    size_t nodesCount() const;
    size_t minesCount() const;
    size_t gridStep() const;

private slots:
    void onNodesCountChanged(int new_value);

private:
    Ui::GraphBoardParametersWidget* ui_;
};

#endif // GRAPH_BOARD_PARAMETERS_WIDGET_HPP
