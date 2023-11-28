#include "main_window.hpp"
#include "ui_main_window.h"
#include "mines_widget.hpp"
#include "new_game_dialog.hpp"
#include "board_factory.hpp"

#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);

    setWindowTitle(QStringLiteral("Mines"));

    new_game_dialog_ = new NewGameDialog { this };
    mines_widget_    = new MinesWidget { this };
    setCentralWidget(mines_widget_);
    connect(mines_widget_, &MinesWidget::gameOver, this, &MainWindow::onGameOver);
    connect(ui_->actionNewGame, &QAction::triggered, this, &MainWindow::showNewGameDialog);
    connect(ui_->actionQuit, &QAction::triggered, this, &QMainWindow::close);

    showNewGameDialog();
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::onGameOver(GameOverDialogAnswer answer)
{
    if (answer == GameOverDialogAnswer::Replay) {
        board_->generate(new_game_dialog_->parametersWidget());
    } else if (answer == GameOverDialogAnswer::NewGame) {
        int result = new_game_dialog_->exec();
        if (result == QDialog::Accepted) {
            auto boardName = new_game_dialog_->selectedBoard();
            board_         = BoardFactory::create(boardName);
            board_->generate(new_game_dialog_->parametersWidget());
            mines_widget_->setBoard(board_.get());
        }
    } else {
        close();
    }
}

void MainWindow::showNewGameDialog()
{
    int result = new_game_dialog_->exec();
    if (result == QDialog::Accepted) {
        auto boardName = new_game_dialog_->selectedBoard();
        board_         = BoardFactory::create(boardName);
        board_->generate(new_game_dialog_->parametersWidget());
        mines_widget_->setBoard(board_.get());
    }
}
