#pragma once

#include <QWidget>
#include "ui_nmino_parameters_widget.h"

class NminoParametersWidget : public QWidget
{
    Q_OBJECT

public:
    NminoParametersWidget(QWidget *parent = nullptr);

private:
    Ui::NminoParametersWidgetClass ui;
};
