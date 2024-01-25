#ifndef MINES_WIDGET_HPP
#define MINES_WIDGET_HPP

#include "board_scene.hpp"
#include "game_state.hpp"
#include "game_over_dialog_answer.hpp"

#include <QWidget>
#include <QGraphicsSceneMouseEvent>
#include <QDialog>

class Board;
class GameOverDialog;
class QGraphicsScene;
class QTimer;

namespace Ui {
class MinesWidget;
}

class MinesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MinesWidget(QWidget* parent = nullptr);
    ~MinesWidget();

    void setBoard(Board* board);

signals:
    void gameOver(GameOverDialogAnswer answer);

private slots:
    void onCellItemClicked(CellItem* cell_item, QGraphicsSceneMouseEvent* event);
    void onCellChanged(Cell* cell);
    void onTimerTimeout();

private:
    void processCellItemClick(CellItem* cell_item, QGraphicsSceneMouseEvent* event);
    void updateFlagsCount();
    void centerView();

private:
    Ui::MinesWidget*        ui_;
    BoardScene*             scene_            = nullptr;
    Board*                  board_            = nullptr;
    QTimer*                 timer_            = nullptr;
    GameOverDialog*         game_over_dialog_ = nullptr;
    static constexpr int    timer_period_     = 1000;
    static constexpr double view_side_        = 50;
};

#endif // MINES_WIDGET_HPP
