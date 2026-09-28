#pragma once

#include "board.hpp"
#include "parameters_widget_holder.hpp"

#include <QTimer>

#include <chrono>

template <std::derived_from<QWidget> ParametersWidgetType>
class AbstractBoard : public Board {
public:
    AbstractBoard();

    const BoardState&     boardState() const override;
    ParametersWidgetType* parametersWidget() const override;
    std::chrono::seconds  elapsedTime() const override;

protected:
    BoardState                                   board_state_;
    QTimer                                       base_timer_;
    std::chrono::steady_clock::time_point        elapsed_time_;
    ParametersWidgetHolder<ParametersWidgetType> parameters_holder_;
};

template <std::derived_from<QWidget> ParametersWidgetType>
AbstractBoard<ParametersWidgetType>::AbstractBoard()
{
    base_timer_.setInterval(1000);
    connect(&base_timer_, &QTimer::timeout, this, &Board::secondPassed);
}

template <std::derived_from<QWidget> ParametersWidgetType>
const BoardState& AbstractBoard<ParametersWidgetType>::boardState() const
{
    return board_state_;
}

template <std::derived_from<QWidget> ParametersWidgetType>
ParametersWidgetType* AbstractBoard<ParametersWidgetType>::parametersWidget() const
{
    return parameters_holder_.parametersWidget();
}

template <std::derived_from<QWidget> ParametersWidgetType>
std::chrono::seconds AbstractBoard<ParametersWidgetType>::elapsedTime() const
{
    using namespace std::chrono;
    return duration_cast<seconds>(steady_clock::now() - elapsed_time_);
}
