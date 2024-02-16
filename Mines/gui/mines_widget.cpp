#include "mines_widget.hpp"
#include "ui_mines_widget.h"
#include "board.hpp"
#include "board_state.hpp"
#include "game_state.hpp"
#include "game_over_dialog.hpp"
#include "game_over_dialog_answer.hpp"
#include "cell.hpp"

#include <QGraphicsScene>
#include <QTimer>

MinesWidget::MinesWidget(QWidget *parent)
    : QWidget { parent }
    , ui_ { new Ui::MinesWidget }
    , scene_ { new BoardScene { this } }
    , timer_ { new QTimer { this } }
    , game_over_dialog_ { new GameOverDialog { this } }
{
    ui_->setupUi(this);

    ui_->minesGraphicsView->setScene(scene_);
    connect(scene_, &BoardScene::cellItemClicked, this, &MinesWidget::onCellItemClicked);
    connect(timer_, &QTimer::timeout, this, &MinesWidget::onTimerTimeout);
}

MinesWidget::~MinesWidget()
{
    delete ui_;
}

void MinesWidget::setBoard(Board *board)
{
    if (board_) {
        disconnect(board_, &Board::cellChanged, this, &MinesWidget::onCellChanged);
    }

    ui_->timeSpinBox->setValue(0);
    board_ = board;
    scene_->clear();
    board->setupScene(scene_);
    connect(board_, &Board::cellChanged, this, &MinesWidget::onCellChanged);
    centerView();
}

void MinesWidget::startGame()
{
    scene_->startAnimation();
}

void MinesWidget::onCellItemClicked(CellItem *cell_item, QGraphicsSceneMouseEvent *event)
{
    processCellItemClick(cell_item, event);

    if (!timer_->isActive()) {
        timer_->start(timer_period_);
    }

    auto game_state = board_->boardState().game_state;
    if (game_state != GameState::Playing) {
        timer_->stop();
        auto answer = game_over_dialog_->exec(game_state);
        emit gameOver(answer);
    }
}

void MinesWidget::onCellChanged(Cell *cell)
{
    scene_->updateCellItemForCell(cell);
}

void MinesWidget::onTimerTimeout()
{
    ui_->timeSpinBox->setValue(ui_->timeSpinBox->value() + 1);
}

void MinesWidget::processCellItemClick(CellItem *cell_item, QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        board_->openCell(cell_item->cell()->id);
    } else if (event->button() == Qt::RightButton) {
        board_->toggleFlag(cell_item->cell()->id);
        updateFlagsCount();
    }
}

void MinesWidget::updateFlagsCount()
{
    ui_->minesSpinBox->setValue(static_cast<int>(board_->flags()));
}

void MinesWidget::centerView()
{
    const auto &scene_rect = scene_->sceneRect();
    scene_->addRect(scene_rect, QPen {Qt::red});
    const auto &border = scene_->sceneRect().adjusted(10,10,-10,-10);
    scene_->addRect(border, QPen { Qt::green });
    scene_->addEllipse(border, QPen { Qt::green });

    if (scene_rect.width() < min_width_ || scene_rect.height() < min_height_) {
        auto min = qMin(scene_rect.width(), scene_rect.height());
        auto x_factor = scene_rect.width() / min_width_;
        auto y_factor = scene_rect.height() / min_height_;
        auto min_factor = qMin(x_factor, y_factor);
        ui_->minesGraphicsView->scale(min_factor, min_factor);
    } else if (scene_rect.width() > min_width_ || scene_rect.height() > min_height_) {
        auto max = qMax(max_width_, max_height_);
        ui_->minesGraphicsView->fitInView(0, 0, max, max, Qt::KeepAspectRatio);
    }
    ui_->minesGraphicsView->centerOn(scene_rect.width() / 2., scene_rect.height() / 2.);
}
