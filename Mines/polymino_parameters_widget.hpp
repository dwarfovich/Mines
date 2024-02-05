#pragma once

#include "ui_polymino_parameters_widget.h"

#include <QWidget>

class PolyminoParametersWidget : public QWidget
{
    Q_OBJECT

public:
    PolyminoParametersWidget(QWidget *parent = nullptr);

private:
    Ui::PolyminoParametersWidgetClass ui;
};
