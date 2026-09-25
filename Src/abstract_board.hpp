#pragma once

#include "board.hpp"
#include "parameters_widget_holder.hpp"

#include <chrono>

template <std::derived_from<QWidget> ParametersWidgetType>
class AbstractBoard : public Board {
public:
    const BoardState& boardState() const override
    {
        return board_state_;
    }

    ParametersWidgetType* parametersWidget() const override
    {
        return holder_.parametersWidget();
    }

    std::chrono::seconds elapsedTime() const override
    {
        return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - elapsed_time_);
    }

protected:
    BoardState                                   board_state_;
    std::chrono::steady_clock::time_point        elapsed_time_;
    ParametersWidgetHolder<ParametersWidgetType> holder_;
};