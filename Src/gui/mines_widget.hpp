#pragma once

#include "board_scene.hpp"
#include "game_over_dialog_answer.hpp"
#include "game_state.hpp"

#include <QDialog>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>

class Board;
class GameOverDialog;
class QGraphicsScene;
class QTimer;

namespace Ui {
class MinesWidget;
}

class MinesWidget : public QWidget {
    Q_OBJECT

public:
    explicit MinesWidget(QWidget* parent = nullptr);
    ~MinesWidget();

    void setBoard(Board* board);
    void startGame();

signals:
    void gameOver(GameOverDialogAnswer answer);

private slots:
    void onCellItemClicked(CellItem* cell_item, QGraphicsSceneMouseEvent* event);
    void onCellClicked(std::size_t id, QGraphicsSceneMouseEvent* event);

    void onCellChanged(Cell* cell);
    void onTimerTimeout();

private:  // methods
    void processCellItemClick(CellItem* cell_item, QGraphicsSceneMouseEvent* event);
    void processCellItemClick(std::size_t id, QGraphicsSceneMouseEvent* event);
    void updateFlagsCount();
    void centerView();

private:  // data
    static constexpr int    update_time_period_ = 1000;
    static constexpr double min_width_ = 360.;
    static constexpr double min_height_ = 360.;
    static constexpr double max_width_ = 400.;
    static constexpr double max_height_ = 400.;

    Ui::MinesWidget*        ui_;
    BoardScene*             scene_ = nullptr;
    Board*                  board_ = nullptr;
    QTimer*                 timer_ = nullptr;
    GameOverDialog*         game_over_dialog_ = nullptr;
};
