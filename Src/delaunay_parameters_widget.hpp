#pragma once

#include <QWidget>

namespace Ui {
class DelaunayParametersWidget;
}

class DelaunayParametersWidget : public QWidget {
    Q_OBJECT

public:
    explicit DelaunayParametersWidget(QWidget* parent = nullptr);
    ~DelaunayParametersWidget();

    std::size_t nodesCount() const;
    std::size_t minesCount() const;

private slots:
    void onNodesCountChanged(int new_value);

private:
    Ui::DelaunayParametersWidget* ui_;
};
