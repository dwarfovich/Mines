#include "board.hpp"

const BoardState& Board::boardState() const
{
    return board_state_;
}

void Board::TakeOwnershipOfParametersWidget(QWidget* widget)
{
    widget->setParent(&dummy_parent_widget_);
}
