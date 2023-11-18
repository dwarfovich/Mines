#ifndef GAME_OVER_DIALOG_HPP
#define GAME_OVER_DIALOG_HPP

#include "game_over_dialog_answer.hpp"
#include "game_state.hpp"

#include <QDialog>

namespace Ui {
class GameOverDialog;
}

class GameOverDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GameOverDialog(QWidget* parent = nullptr);
    ~GameOverDialog();

    GameOverDialogAnswer exec(GameState game_state);

private:
    void updateMessage(GameState game_state);

private:
    Ui::GameOverDialog* ui_;
    GameOverDialogAnswer answer_;
};

#endif // GAME_OVER_DIALOG_HPP
