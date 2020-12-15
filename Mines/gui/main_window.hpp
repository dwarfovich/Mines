#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "game_over_dialog_answer.hpp"

#include <QMainWindow>

#include <memory>

class IBoard;
class MinesWidget;
class NewGameDialog;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onGameOver(GameOverDialogAnswer answer);
    void showNewGameDialog();

private:
    Ui::MainWindow* ui_;
    MinesWidget* mines_widget_;
    NewGameDialog* new_game_dialog_;
    std::unique_ptr<IBoard> board_;
};

#endif // MAINWINDOW_HPP
