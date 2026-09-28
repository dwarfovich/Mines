#include "polyomino_parameters_widget.hpp"

PolyominoParametersWidget::PolyominoParametersWidget(QWidget* parent) : QWidget{parent}
{
    ui_.setupUi(this);
    connect(ui_.widthSpinBox,
            qOverload<int>(&QSpinBox::valueChanged),
            this,
            &PolyominoParametersWidget::updateMaxPolyominoSizeSpinBox);
    connect(ui_.heightSpinBox,
            qOverload<int>(&QSpinBox::valueChanged),
            this,
            &PolyominoParametersWidget::updateMaxPolyominoSizeSpinBox);
    connect(ui_.maxPolyominoSizeSpinBox,
            qOverload<int>(&QSpinBox::valueChanged),
            this,
            &PolyominoParametersWidget::updateMaxPolyominoSizeSpinBox);

    updateMaxPolyominoSizeSpinBox();
}

size_t PolyominoParametersWidget::width() const
{
    return ui_.widthSpinBox->value();
}

size_t PolyominoParametersWidget::height() const
{
    return ui_.heightSpinBox->value();
}

size_t PolyominoParametersWidget::maxPolyominoSize() const
{
    return ui_.maxPolyominoSizeSpinBox->value();
}

size_t PolyominoParametersWidget::minesCount() const
{
    return ui_.minesSpinBox->value();
}

void PolyominoParametersWidget::updateMaxPolyominoSizeSpinBox()
{
    const auto subCellCount = width() * height();
    ui_.maxPolyominoSizeSpinBox->setMaximum(subCellCount - 1);
    updateMinesSpinbox();
}

void PolyominoParametersWidget::updateMinesSpinbox()
{
    const std::size_t width = ui_.widthSpinBox->value();
    const std::size_t height = ui_.heightSpinBox->value();

    const std::size_t denominator = (ui_.maxPolyominoSizeSpinBox->value() - 1) / 2;
    std::size_t       averageCellsCount = (width * height) / (denominator == 0 ? 1 : denominator);
    if (averageCellsCount == 0) {
        averageCellsCount = 1;
    }
    ui_.minesSpinBox->setMaximum(averageCellsCount - 1);
}