#include "delaunay_parameters_widget.hpp"
#include "ui_delaunay_parameters_widget.h"

DelaunayParametersWidget::DelaunayParametersWidget(QWidget *parent)
    : QWidget(parent), ui_(new Ui::DelaunayParametersWidget)
{
    ui_->setupUi(this);

    connect(ui_->nodesSpinBox,
            qOverload<int>(&QSpinBox::valueChanged),
            this,
            &DelaunayParametersWidget::onNodesCountChanged);
}

DelaunayParametersWidget::~DelaunayParametersWidget()
{
    delete ui_;
}

size_t DelaunayParametersWidget::nodesCount() const
{
    return static_cast<size_t>(ui_->nodesSpinBox->value());
}

size_t DelaunayParametersWidget::minesCount() const
{
    return static_cast<size_t>(ui_->minesSpinBox->value());
}

void DelaunayParametersWidget::onNodesCountChanged(int new_value)
{
    ui_->minesSpinBox->setMaximum(new_value - 1);
}
