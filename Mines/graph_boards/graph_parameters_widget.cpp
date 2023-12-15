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
