#ifndef IBOARD_HPP
#define IBOARD_HPP

#include "utils.hpp"
#include "board_state.hpp"

#include <QObject>
#include <QWidget>

#include <memory>

class Cell;
class BoardScene;
class QWidget;

class Board : public QObject
{
    Q_OBJECT

public:
    virtual ~Board() = default;

    virtual const QString& id() const                    = 0;
    virtual const QString& name() const                  = 0;
    virtual size_t         flags() const                 = 0;
    virtual void           generate()                    = 0;
    virtual const Cell*    cellById(size_t id) const     = 0;
    virtual void           openCell(size_t id)           = 0;
    virtual void           toggleFlag(size_t id)         = 0;
    virtual void           setupScene(BoardScene* scene) = 0;
    virtual QWidget*       parametersWidget() const      = 0;

    const BoardState& boardState() const;

signals:
    void cellChanged(Cell* cell);

protected:
    BoardState board_state_;
};

#endif // IBOARD_HPP
