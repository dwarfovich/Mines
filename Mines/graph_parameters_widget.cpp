#include "graph_parameters_widget.h"

GraphParametersWidget::GraphParametersWidget(QWidget *parent)
    : QWidget(parent), ui_(new Ui::GraphParametersWidgetClass())
{
    ui_->setupUi(this);

    connect(ui_->nodesSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &GraphParametersWidget::onNodesCountChanged);
}

void GraphParametersWidget::onNodesCountChanged(int count)
{
    ui_->minesSpinBox->setMaximum(count - 1);
    ui_->maximumNeighborsSpinBox->setMaximum(count - 1);
}

GraphParametersWidget::~GraphParametersWidget()
{
    delete ui_;
}

std::size_t GraphParametersWidget::nodesCount() const
{
    return ui_->nodesSpinBox->value();
}

std::size_t GraphParametersWidget::minesCount() const
{
    return ui_->minesSpinBox->value();
}

std::size_t GraphParametersWidget::maximumNeighbors()
{
    return ui_->maximumNeighborsSpinBox->value();
}

bool GraphParametersWidget::disjointGraphAllowed()
{
    return ui_->allowDisjointGraphCheckBox->isChecked();
}
