#include "dynamic_graph_parameters_widget.h"

DynamicGraphParametersWidget::DynamicGraphParametersWidget(QWidget *parent) : QWidget(parent)
{
    ui_.setupUi(this);
    connect(ui_.nodesSpinBox, &QSpinBox::valueChanged, this, &DynamicGraphParametersWidget::onNodesCountChanged);
    connect(ui_.speedSlider, &QSlider::valueChanged, this, &DynamicGraphParametersWidget::onSpeedValueChanged);
    connect(ui_.speedSpinBox, &QSpinBox::valueChanged, this, &DynamicGraphParametersWidget::onSpeedValueChanged);
}

void DynamicGraphParametersWidget::onNodesCountChanged(int count)
{
    ui_.minesSpinBox->setMaximum(count - 1);
    ui_.maximumNeighborsSpinBox->setMaximum(count - 1);
}

size_t DynamicGraphParametersWidget::nodesCount() const
{
    return ui_.nodesSpinBox->value();
}

size_t DynamicGraphParametersWidget::minesCount() const
{
    return ui_.minesSpinBox->value();
}

size_t DynamicGraphParametersWidget::maximumNeighbors() const
{
    return ui_.maximumNeighborsSpinBox->value();
}

size_t DynamicGraphParametersWidget::speed() const
{
    return ui_.speedSpinBox->value();
}

bool DynamicGraphParametersWidget::allowDisjointGraph() const
{
    return ui_.allowDisjointGraphCheckBox->isChecked();
}

void DynamicGraphParametersWidget::onSpeedValueChanged(int speed)
{
    const QSignalBlocker sliderBlocker {ui_.speedSlider};
    const QSignalBlocker spinBoxBlocker { ui_.speedSpinBox };
    ui_.speedSlider->setValue(speed);
    ui_.speedSpinBox->setValue(speed);
}
