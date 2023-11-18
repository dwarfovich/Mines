#include "new_game_dialog.hpp"
#include "ui_new_game_dialog.h"
#include "board_factory.hpp"

#include <QDebug>
#define DEB qDebug()

NewGameDialog::NewGameDialog(QWidget *parent)
    : QDialog{parent}
    , ui_(new Ui::NewGameDialog)
{
    ui_->setupUi(this);
    connect(ui_->boardsComboBox, qOverload<int>(&QComboBox::currentIndexChanged),
            this, &NewGameDialog::onNewBoardSelected);
    ui_->parametersWidgetFrame->setLayout(new QHBoxLayout {});
    initializeBoardCombo();

    connect(ui_->startButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui_->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

NewGameDialog::~NewGameDialog()
{
    delete ui_;
}

void NewGameDialog::onNewBoardSelected(int index)
{
    const auto& boardName = ui_->boardsComboBox->itemText(index);
    auto board = BoardFactory::create(boardName);
    if (board) {
        delete parameters_widget_;
        parameters_widget_ = board->createParametersWidget();
        if (parameters_widget_){
            auto layout = ui_->parametersWidgetFrame->layout();
            layout->addWidget(parameters_widget_);
        } else {
            Q_ASSERT(false);
        }
    } else {
        Q_ASSERT(false);
    }
}

QString NewGameDialog::selectedBoard() const
{
    return ui_->boardsComboBox->currentText();
}

QWidget *NewGameDialog::parametersWidget() const
{
    return parameters_widget_;
}

void NewGameDialog::initializeBoardCombo()
{
    QStringList names;
    for (const auto& [name, board] : BoardFactory::boards()) {
        names << name;
    }
    ui_->boardsComboBox->addItems(names);
}
