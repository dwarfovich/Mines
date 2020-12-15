#include "game_over_dialog.hpp"
#include "ui_game_over_dialog.h"

GameOverDialog::GameOverDialog(QWidget *parent)
    : QDialog{parent}
    , ui_{new Ui::GameOverDialog}
{
    ui_->setupUi(this);
    connect(ui_->quitButton, &QPushButton::clicked, [this] { answer_ = GameOverDialogAnswer::Quit; close(); } );
    connect(ui_->replayButton, &QPushButton::clicked, [this] { answer_ = GameOverDialogAnswer::Replay; close(); } );
    connect(ui_->newGameButton, &QPushButton::clicked, [this] { answer_ = GameOverDialogAnswer::NewGame; close(); } );
}

GameOverDialog::~GameOverDialog()
{
    delete ui_;
}

GameOverDialogAnswer GameOverDialog::exec(GameState game_state)
{
    updateMessage(game_state);
    QDialog::exec();
    return answer_;
}

void GameOverDialog::updateMessage(GameState game_state)
{
    if (game_state == GameState::Loose) {
        ui_->messageLabel->setText(tr("You lose! Do you want to try again?"));
    } else {
        ui_->messageLabel->setText(tr("You win! Do you want to try again?"));
    }
}
