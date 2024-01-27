#include "hex_parameters_widget.hpp"

HexParametersWidget::HexParametersWidget(QWidget *parent) : QWidget(parent)
{
    ui_.setupUi(this);
    connect(
        ui_.heightSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &HexParametersWidget::updateMinesSpinbox);
    connect(ui_.widthSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &HexParametersWidget::updateMinesSpinbox);
}

int HexParametersWidget::boardWidth() const
{
    return ui_.widthSpinBox->value();
}

int HexParametersWidget::boardHeight() const
{
    return ui_.heightSpinBox->value();
}

int HexParametersWidget::mines() const
{
    return ui_.minesSpinBox->value();
}

void HexParametersWidget::updateMinesSpinbox()
{
    auto cells = ui_.heightSpinBox->value() * ui_.widthSpinBox->value();
    ui_.minesSpinBox->setMaximum(cells - 1);
}
