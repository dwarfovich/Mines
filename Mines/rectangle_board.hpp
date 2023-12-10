#ifndef RECTANGLEBOARD_HPP
#define RECTANGLEBOARD_HPP

#include "standard_board.hpp"

class RectangleBoard : public StandardBoard
{
    Q_OBJECT

public:
    std::unique_ptr<Board> create() const override;
    const QString         &id() const override;
    const QString         &name() const override;
    void                   drawBoard(BoardScene *scene) override;
    void                   generate() override;
    QWidget               *parametersWidget() const override;

protected:
    std::vector<size_t>                     neighborIds(size_t id) const override;
    mutable RectangleBoardParametersWidget *parameters_widget_ = nullptr;
};

#endif // RECTANGLEBOARD_HPP
