#include "graph_board_parameters_widget.hpp"
#include "ui_graph_board_parameters_widget.h"

GraphBoardParametersWidget::GraphBoardParametersWidget(QWidget *parent) :
    QWidget(parent),
    ui_(new Ui::GraphBoardParametersWidget)
{
    ui_->setupUi(this);

    connect(ui_->nodesSpinBox, qOverload<int>(&QSpinBox::valueChanged),
            this, &GraphBoardParametersWidget::onNodesCountChanged);
}

GraphBoardParametersWidget::~GraphBoardParametersWidget()
{
    delete ui_;
}

size_t GraphBoardParametersWidget::nodesCount() const
{
    return static_cast<size_t>(ui_->nodesSpinBox->value());
}

size_t GraphBoardParametersWidget::minesCount() const
{
    return static_cast<size_t>(ui_->minesSpinBox->value());
}

size_t GraphBoardParametersWidget::gridStep() const
{
    return static_cast<size_t>(ui_->gridStepSpinBox->value());
}

size_t GraphBoardParametersWidget::sparseMultiplier() const
{
    return static_cast<size_t>(ui_->sparseMultiplierSpinBox->value());
}

void GraphBoardParametersWidget::onNodesCountChanged(int new_value)
{
    ui_->minesSpinBox->setMaximum(new_value - 1);
}
