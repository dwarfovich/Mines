#pragma once

#include "board_state.hpp"
#include "utils.hpp"

#include <QObject>
#include <QWidget>

#include <memory>

class Cell;
class BoardScene;

class Board : public QObject {
    Q_OBJECT

public:
    Board() = default;
    virtual ~Board() = default;
    Board(const Board&) = delete;
    Board(Board&&) = delete;
    Board& operator=(const Board&) = delete;
    Board& operator=(Board&&) = delete;

    virtual const BoardState& boardState() const = 0;
    virtual const QString& id() const = 0;
    virtual const QString& name() const = 0;
    virtual std::size_t    flags() const = 0;
    virtual void           generate() = 0;
    virtual const Cell*    cellById(std::size_t id) const = 0;
    virtual void           openCell(std::size_t id) = 0;
    virtual void           toggleFlag(std::size_t id) = 0;
    virtual void           setupScene(BoardScene* scene) = 0;
    virtual QWidget* parametersWidget() const = 0;

signals:
    void cellChanged(Cell* cell);
};