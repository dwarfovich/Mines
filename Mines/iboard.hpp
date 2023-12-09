#ifndef IBOARD_HPP
#define IBOARD_HPP

#include "utils.hpp"
#include "board_state.hpp"

#include <QVariant>

#include <memory>
#include <unordered_map>

class ParametersMap;
class Cell;
class BoardScene;

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

class IBoard : public QObject
{
    Q_OBJECT

public:
    virtual ~IBoard() {}

    virtual std::unique_ptr<IBoard> create() const                       = 0;
    virtual QObject*                toQObject()                          = 0;
    virtual const QString&          id() const                           = 0;
    virtual const QString&          name() const                         = 0;
    virtual QWidget*                createParametersWidget() const       = 0;
    virtual size_t                  flags() const                        = 0;
    virtual void                    generate(QWidget* parameters_widget) = 0;
    virtual const Cell*             cellById(size_t id) const            = 0;
    virtual void                    openCell(size_t id)                  = 0;
    virtual void                    toggleFlag(size_t id)                = 0;
    virtual void                    drawBoard(BoardScene* scene)         = 0;
    virtual const BoardState&       boardState() const                   = 0;

signals:
    void cellChanged(Cell* cell);
};

#endif // IBOARD_HPP
