#ifndef RECTANGLE_BOARD_PARAMETERS_WIDGET_HPP
#define RECTANGLE_BOARD_PARAMETERS_WIDGET_HPP

#include <QWidget>

namespace Ui {
class RectangleBoardParametersWidget;
}

class RectangleBoardParametersWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RectangleBoardParametersWidget(QWidget *parent = nullptr);
    ~RectangleBoardParametersWidget();

    int boardWidth() const;
    int boardHeight() const;
    int mines() const;

private slots:
    void updateMinesSpinbox();

private:
    Ui::RectangleBoardParametersWidget* ui_;
};

#endif // RECTANGLE_BOARD_PARAMETERS_WIDGET_HPP
