#pragma once

#include "board.hpp"
#include "parameters_widget_holder.hpp"

template<std::derived_from<QWidget> ParametersWidgetType>
class AbstractBoard : public Board {
public:
    const BoardState& boardState() const override
    {
        return board_state_;
    }

    ParametersWidgetType* parametersWidget() const{
        return holder_.parametersWidget();
    }

protected:
    BoardState board_state_;
    ParametersWidgetHolder<ParametersWidgetType> holder_;
};