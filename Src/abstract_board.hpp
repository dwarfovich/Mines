#pragma once

#include "board.hpp"
#include "parameters_widget_holder.hpp"

#include <QTimer>

#include <chrono>

template <std::derived_from<QWidget> ParametersWidgetType>
class AbstractBoard : public Board {
public:
    AbstractBoard(){
        base_timer_.setInterval(1000);
        connect(&base_timer_, &QTimer::timeout, this, &Board::secondPassed);
    }

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
        using namespace std::chrono;
        return duration_cast<seconds>(steady_clock::now() - elapsed_time_);
    }

protected:
    BoardState                                   board_state_;
    QTimer                                       base_timer_;
    std::chrono::steady_clock::time_point        elapsed_time_;
    ParametersWidgetHolder<ParametersWidgetType> holder_;
};