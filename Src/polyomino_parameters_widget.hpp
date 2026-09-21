#pragma once

#include "ui_polyomino_parameters_widget.h"

#include <QWidget>

class PolyominoParametersWidget : public QWidget
{
    Q_OBJECT

public:
    PolyominoParametersWidget(QWidget *parent = nullptr);

    size_t width() const;
    size_t height() const;
    size_t maxPolyominoSize() const;
    size_t minesCount() const;

private slots:
    void updateMaxPolyominoSizeSpinBox();
    void updateMinesSpinbox();

private: // data
    Ui::PolyominoParametersWidgetClass ui_;
};
