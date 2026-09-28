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
    void onCellClicked(std::size_t id, QGraphicsSceneMouseEvent* event);
    void onCellChanged(const Cell* cell);
    void onTimerTimeout();

private:  // methods
    void processCellItemClick(std::size_t id, QGraphicsSceneMouseEvent* event);
    void updateFlagsCount();
    void centerView();

private:  // data
    static constexpr double min_width_ = 360.;
    static constexpr double min_height_ = 360.;
    static constexpr double max_width_ = 400.;
    static constexpr double max_height_ = 400.;

    Ui::MinesWidget*        ui_;
    BoardScene*             scene_ = nullptr;
    Board*                  board_ = nullptr;
    GameOverDialog*         game_over_dialog_ = nullptr;
};
