#pragma once

#include "id_based_board.hpp"
#include "rectangle_board_parameters_widget.hpp"

class RectangleBoard : public IdBasedBoard<Cell, RectangleBoardParametersWidget> {
    Q_OBJECT

public:
    const QString& id() const override;
    const QString& name() const override;
    void           setupScene(BoardScene* scene) override;
    void           generate() override;
    //QWidget*       parametersWidget() const override;

protected:
    std::vector<std::size_t> neighborIds(std::size_t id) const override;

protected:  // data
    std::size_t                             height_ = 0;
    std::size_t                             width_ = 0;
    //mutable RectangleBoardParametersWidget* parameters_widget_ = nullptr;
};