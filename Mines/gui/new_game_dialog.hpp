#ifndef NEW_GAME_DIALOG_HPP
#define NEW_GAME_DIALOG_HPP

#include "../utils.hpp"

#include <QDialog>

class BoardCollection;
class QHBoxLayout;

namespace Ui {
class NewGameDialog;
}

class NewGameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewGameDialog(BoardCollection* collection, QWidget* parent = nullptr);
    ~NewGameDialog();

    QString  selectedBoard() const;
    QWidget* parametersWidget() const;

private slots:
    void onNewBoardSelected(int index);

private:
    void initializeBoardCombo();

private:
    Ui::NewGameDialog* ui_;
    QWidget*           parameters_widget_ = nullptr;
    QHBoxLayout*       parameters_layout_ = nullptr;
    BoardCollection*   collection_        = nullptr;
};

#endif // NEW_GAME_DIALOG_HPP
