#ifndef RECTANGLEBOARD_HPP
#define RECTANGLEBOARD_HPP

#include "standard_board.hpp"

class RectangleBoard : public StandardBoard
{
    Q_OBJECT

public:
    std::unique_ptr<IBoard> clone() const override;
    QObject *toQObject() override;
    const QString &id() const override;
    const QString &name() const override;
    void drawBoard(BoardScene *scene) override;

protected:
    std::vector<size_t> neighborIds(size_t id) const override;
};

#endif // RECTANGLEBOARD_HPP
