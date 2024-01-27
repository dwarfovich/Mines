#include "nmino_board.hpp"
#include "nmino_parameters_widget.h"

NminoBoard::NminoBoard() : random_generator_ { random_device_() }
{
}

const QString& NminoBoard::id() const
{
    static const QString id = "Nmino";
    return id;
}

const QString& NminoBoard::name() const
{
    static const QString name = "Nmino";
    return name;
}

void NminoBoard::generate()
{
    const size_t width          = 10;
    const size_t height         = 10;
    const size_t max_nmino_size = 5;

    std::uniform_int_distribution<> distribution(1, max_nmino_size);

}

void NminoBoard::setupScene(BoardScene* scene)
{
}

QWidget* NminoBoard::parametersWidget() const
{
    if (!parameters_widget_) {
        parameters_widget_ = new NminoParametersWidget { &dummy_parent_widget_ };
    }

    return parameters_widget_;
}

std::vector<size_t> NminoBoard::neighborIds(size_t id) const
{
    return std::vector<size_t>();
}
