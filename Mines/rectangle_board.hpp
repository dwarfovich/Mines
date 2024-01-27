#ifndef RECTANGLEBOARD_HPP
#define RECTANGLEBOARD_HPP

#include "id_based_board.hpp"

class RectangleBoard : public IdBasedBoard<Cell>
{
    Q_OBJECT

public:
    const QString &id() const override;
    const QString &name() const override;
    void           setupScene(BoardScene *scene) override;
    void           generate() override;
    QWidget       *parametersWidget() const override;

protected:
    std::vector<size_t> neighborIds(size_t id) const override;

protected: // data
    size_t                                  height_            = 0;
    size_t                                  width_             = 0;
    mutable RectangleBoardParametersWidget *parameters_widget_ = nullptr;
};

#endif // RECTANGLEBOARD_HPP
