#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "game_over_dialog_answer.hpp"
#include "board_collection.hpp"
#include "new_game_dialog.hpp"

#include <QMainWindow>

#include <memory>

class Board;
class MinesWidget;
class NGD;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(std::unique_ptr<BoardCollection> collection, QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onGameOver(GameOverDialogAnswer answer);
    void showNewGameDialog();

private:
    Ui::MainWindow*                  ui_;
    MinesWidget*                     mines_widget_;
    NewGameDialog*                   new_game_dialog_   = nullptr;
    QHBoxLayout*                     parameters_layout_ = nullptr;
    Board*                           board_             = nullptr;
    std::unique_ptr<BoardCollection> board_collection_;
};

#endif // MAINWINDOW_HPP
