#ifndef HEXBOARD_HPP
#define HEXBOARD_HPP

#include "standard_board.hpp"

class HexBoard : public StandardBoard
{
    Q_OBJECT

public:
    std::unique_ptr<IBoard> create() const override;
    QObject                *toQObject() override;
    const QString          &id() const override;
    const QString          &name() const override;
    void                    drawBoard(BoardScene *scene) override;

protected:
    std::vector<size_t> neighborIds(size_t id) const override;
};

#endif // HEXBOARD_HPP
