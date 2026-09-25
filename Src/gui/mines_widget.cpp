#include "mines_widget.hpp"
#include "board.hpp"
#include "board_state.hpp"
#include "cell.hpp"
#include "game_over_dialog.hpp"
#include "game_over_dialog_answer.hpp"
#include "game_state.hpp"
#include "ui_mines_widget.h"

#include <QGraphicsScene>
#include <QTimer>

MinesWidget::MinesWidget(QWidget* parent)
    : QWidget{parent},
      ui_{new Ui::MinesWidget},
      scene_{new BoardScene{this}},
      timer_{new QTimer{this}},
      game_over_dialog_{new GameOverDialog{this}}
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

void MinesWidget::setBoard(Board* board)
{
    if (board_) {
        disconnect(board_, &Board::cellChanged, this, &MinesWidget::onCellChanged);
    }

    ui_->timeSpinBox->setValue(0);
    board_ = board;
    board_->generate();
    scene_->clear();
    board->setupScene(scene_);
    connect(board_, &Board::cellChanged, this, &MinesWidget::onCellChanged);
    centerView();
}

void MinesWidget::startGame()
{
    scene_->startAnimation();
}

void MinesWidget::onCellItemClicked(CellItem* cell_item, QGraphicsSceneMouseEvent* event)
{
    processCellItemClick(cell_item, event);

    if (!timer_->isActive()) {
        timer_->start(update_time_period_);
    }

    auto game_state = board_->boardState().game_state;
    if (game_state != GameState::Playing) {
        timer_->stop();
        scene_->stopAnimation();
        auto answer = game_over_dialog_->exec(game_state);
        emit gameOver(answer);
    }
}

void MinesWidget::onCellChanged(Cell* cell)
{
    scene_->updateCellItemForCell(cell);
}

void MinesWidget::onTimerTimeout()
{
    const auto elapsed_time = board_->elapsedTime().count();
    if (elapsed_time > static_cast<decltype(elapsed_time)>(std::numeric_limits<int>::max())) {
        ui_->timeSpinBox->setValue(std::numeric_limits<int>::max());
    } else {
        ui_->timeSpinBox->setValue(static_cast<int>(elapsed_time));
    }
}

void MinesWidget::processCellItemClick(CellItem* cell_item, QGraphicsSceneMouseEvent* event)
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
    const auto& scene_rect = scene_->sceneRect();
    const auto  view_size = ui_->minesGraphicsView->viewport()->size();
    const auto scale_x = view_size.width() / scene_rect.width();
    const auto scale_y = view_size.height() / scene_rect.height();
    const auto scale_factor = std::min(scale_x, scale_y);
    ui_->minesGraphicsView->resetTransform();
    ui_->minesGraphicsView->scale(scale_factor, scale_factor);
    ui_->minesGraphicsView->centerOn(scene_rect.center());
   
}
