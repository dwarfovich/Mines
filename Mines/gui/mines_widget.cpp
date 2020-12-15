#include "mines_widget.hpp"
#include "ui_mines_widget.h"
#include "iboard.hpp"
#include "board_state.hpp"
#include "game_state.hpp"
#include "game_over_dialog.hpp"
#include "game_over_dialog_answer.hpp"
#include "cell.hpp"

#include <QGraphicsScene>
#include <QTimer>

#include <QDebug>
#define DEB qDebug()

MinesWidget::MinesWidget(QWidget* parent)
    : QWidget{parent}
    , ui_{new Ui::MinesWidget}
    , scene_{new BoardScene {this}}
    , timer_{new QTimer {this}}
    , game_over_dialog_{new GameOverDialog {this}}
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

void MinesWidget::setBoard(IBoard *board)
{
    ui_->timeSpinBox->setValue(0);
    board_ = board;
    scene_->clear();
    board->drawBoard(scene_);
//    connect(board_->toQObject(), SIGNAL(cellChanged(Cell*)), this, SLOT(onCellChanged(Cell*)));
    connect(board_, &IBoard::cellChanged, this, &MinesWidget::onCellChanged);
}

void MinesWidget::onCellItemClicked(CellItem *cell_item, QGraphicsSceneMouseEvent *event)
{
    processCellItemClick(cell_item, event);

    if (!timer_->isActive()) {
        timer_->start(1000);
    }

    auto game_state = board_->boardState().game_state;
    if(game_state != GameState::Playing) {
        timer_->stop();
        auto answer = game_over_dialog_->exec(game_state);
        emit gameOver(answer);
    }
}

void MinesWidget::onCellChanged(Cell *cell)
{
    DEB << "cell item changed";
    scene_->updateCellItemForCell(cell);
}

void MinesWidget::onTimerTimeout()
{
    ui_->timeSpinBox->setValue(ui_->timeSpinBox->value() + 1);
}

void MinesWidget::processCellItemClick(CellItem *cell_item, QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
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
