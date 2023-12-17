#include "graph_parameters_widget.h"

GraphParametersWidget::GraphParametersWidget(QWidget *parent)
    : QWidget(parent), ui_(new Ui::GraphParametersWidgetClass())
{
    ui_->setupUi(this);
}

GraphParametersWidget::~GraphParametersWidget()
{
    delete ui_;
}

size_t GraphParametersWidget::nodesCount() const
{
    return ui_->nodesSpinBox->value();
}

size_t GraphParametersWidget::minesCount() const
{
    return ui_->minesSpinBox->value();
}

size_t GraphParametersWidget::maximumNeighbors()
{
    return 3;
}

bool GraphParametersWidget::disjointGraphAllowed()
{
    return ui_->allowDisjointGraphCheckBox->isChecked();
}
