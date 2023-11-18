#include "rectangle_board_parameters_widget.hpp"
#include "ui_rectangle_board_parameters_widget.h"

#include <QSpinBox>

RectangleBoardParametersWidget::RectangleBoardParametersWidget(QWidget *parent)
    : QWidget{parent}
    , ui_(new Ui::RectangleBoardParametersWidget)
{
    ui_->setupUi(this);
    connect(ui_->heightSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &RectangleBoardParametersWidget::updateMinesSpinbox);
    connect(ui_->widthSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &RectangleBoardParametersWidget::updateMinesSpinbox);
}

RectangleBoardParametersWidget::~RectangleBoardParametersWidget()
{
    delete ui_;
}

int RectangleBoardParametersWidget::boardWidth() const
{
    return ui_->widthSpinBox->value();
}

int RectangleBoardParametersWidget::boardHeight() const
{
    return ui_->heightSpinBox->value();
}

int RectangleBoardParametersWidget::mines() const
{
    return ui_->minesSpinBox->value();
}

void RectangleBoardParametersWidget::updateMinesSpinbox()
{
    auto cells = ui_->heightSpinBox->value() * ui_->widthSpinBox->value();
    ui_->minesSpinBox->setMaximum(cells - 1);
}
