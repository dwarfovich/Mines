#pragma once

#include "ui_hex_parameters_widget.h"

#include <QWidget>

class HexParametersWidget : public QWidget
{
    Q_OBJECT

public:
    HexParametersWidget(QWidget *parent = nullptr);

    int boardWidth() const;
    int boardHeight() const;
    int mines() const;

private slots:
    void updateMinesSpinbox();

private:
    Ui::HexParametersWidgetClass ui_;
};
