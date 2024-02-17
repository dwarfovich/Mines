#include "new_game_dialog.hpp"
#include "ui_new_game_dialog.h"
#include "board_collection.hpp"

#include <QDebug>
#define DEB qDebug()

NewGameDialog::NewGameDialog(BoardCollection* collection, QWidget* parent)
    : QDialog { parent }, ui_(new Ui::NewGameDialog), collection_ { collection }
{
    ui_->setupUi(this);
    connect(
        ui_->boardsComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &NewGameDialog::onNewBoardSelected);

    parameters_layout_ = new QHBoxLayout();
    ui_->parametersWidgetGroupBox->setLayout(parameters_layout_);
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
    auto        board     = collection_->get(boardName);
    if (board) {
        if (parameters_widget_) {
            parameters_layout_->removeWidget(parameters_widget_);
            board_->TakeOwnershipOfParametersWidget(parameters_widget_);
        }
        parameters_widget_ = board->parametersWidget();
        Q_ASSERT(parameters_widget_);
        parameters_layout_->addWidget(parameters_widget_);
        board_ = board;
    } else {
        Q_ASSERT(false);
    }
}

QString NewGameDialog::selectedBoard() const
{
    return ui_->boardsComboBox->currentText();
}

QWidget* NewGameDialog::parametersWidget() const
{
    return parameters_widget_;
}

void NewGameDialog::initializeBoardCombo()
{
    ui_->boardsComboBox->setInsertPolicy(QComboBox::InsertAlphabetically);
    QStringList names;
    for (const auto& [name, board] : collection_->boards()) {
        names << name;
    }
    ui_->boardsComboBox->addItems(names);
    ui_->boardsComboBox->model()->sort(0);
    ui_->boardsComboBox->setCurrentIndex(0);
}
