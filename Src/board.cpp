#include "board.hpp"

const BoardState& Board::boardState() const
{
    return board_state_;
}

void Board::TakeOwnershipOfParametersWidget(QWidget* widget)
{
    widget->setParent(&settings_widget_holder_);
}
