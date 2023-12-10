#ifndef HEXBOARD_HPP
#define HEXBOARD_HPP

#include "rectangle_board.hpp"

class HexBoard : public RectangleBoard
{
    Q_OBJECT

public:
    const QString &id() const override;
    const QString &name() const override;
    void           setupScene(BoardScene *scene) override;
    void           generate() override;
    QWidget       *parametersWidget() const override;

protected:
    std::vector<size_t>                     neighborIds(size_t id) const override;
    mutable RectangleBoardParametersWidget *parameters_widget_ = nullptr;
};

#endif // HEXBOARD_HPP
