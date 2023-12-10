#ifndef DELAUNAY_PARAMETERS_WIDGET_HPP
#define DELAUNAY_PARAMETERS_WIDGET_HPP

#include <QWidget>

namespace Ui {
class DelaunayParametersWidget;
}

class DelaunayParametersWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DelaunayParametersWidget(QWidget* parent = nullptr);
    ~DelaunayParametersWidget();

    size_t nodesCount() const;
    size_t minesCount() const;

private slots:
    void onNodesCountChanged(int new_value);

private:
    Ui::DelaunayParametersWidget* ui_;
};

#endif // DELAUNAY_PARAMETERS_WIDGET_HPP
