#pragma once

#include "ui_polymino_parameters_widget.h"

#include <QWidget>

class PolyominoParametersWidget : public QWidget
{
    Q_OBJECT

public:
    PolyominoParametersWidget(QWidget *parent = nullptr);

private:
    Ui::PolyminoParametersWidgetClass ui;
};
