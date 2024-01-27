#pragma once

#include "id_based_board.hpp"

#include <random>

class NminoParametersWidget;

class NminoBoard : public IdBasedBoard
{
public:
    NminoBoard();

    const QString& id() const override;
    const QString& name() const override;
    void           generate() override;
    void           setupScene(BoardScene* scene) override;
    QWidget*       parametersWidget() const override;
    std::vector<size_t> neighborIds(size_t id) const override;

    private:
    mutable NminoParametersWidget* parameters_widget_ = nullptr;
        std::random_device             random_device_;
        std::mt19937                   random_generator_;

};
