#include "main_window.hpp"
#include "ui_main_window.h"
#include "mines_widget.hpp"
#include "new_game_dialog.hpp"
#include "board_collection.hpp"

#include <QVBoxLayout>
#include <QTimer>

MainWindow::MainWindow(std::unique_ptr<BoardCollection> collection, QWidget* parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow), board_collection_ { std::move(collection) }
{
    Q_ASSERT(board_collection_);

    ui_->setupUi(this);
    setWindowTitle(QStringLiteral("Mines"));

    new_game_dialog_ = new NewGameDialog { board_collection_.get(), this };
    mines_widget_    = new MinesWidget { this };
    setCentralWidget(mines_widget_);
    connect(mines_widget_, &MinesWidget::gameOver, this, &MainWindow::onGameOver);
    connect(ui_->actionNewGame, &QAction::triggered, this, &MainWindow::showNewGameDialog);
    connect(ui_->actionQuit, &QAction::triggered, this, &QMainWindow::close);

    QTimer::singleShot(50, this, &MainWindow::showNewGameDialog);
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::onGameOver(GameOverDialogAnswer answer)
{
    if (answer == GameOverDialogAnswer::Replay) {
        board_->generate();
        mines_widget_->setBoard(board_);
    } else if (answer == GameOverDialogAnswer::NewGame) {
        int result = new_game_dialog_->exec();
        if (result == QDialog::Accepted) {
            auto boardName = new_game_dialog_->selectedBoard();
            board_         = board_collection_->get(boardName);
            board_->generate();
            mines_widget_->setBoard(board_);
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
        board_         = board_collection_->get(boardName);
        board_->generate();
        mines_widget_->setBoard(board_);
    }
}
