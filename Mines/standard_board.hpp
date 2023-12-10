#ifndef STANDARDBOARD_HPP
#define STANDARDBOARD_HPP

#include "board.hpp"
#include "cell.hpp"

class RectangleBoardParametersWidget;

class StandardBoard : public Board
{
    Q_OBJECT

public:
    size_t            flags() const override;
    const Cell*       cellById(size_t id) const override;
    void              openCell(size_t id) override;
    void              toggleFlag(size_t id) override;
    const BoardState& boardState() const override;

    // QWidget*          parametersWidget() const override;

protected: // methods
    virtual Cell*               cellById(size_t id);
    virtual void                relocateMine(Cell* cell);
    virtual void                reveal();
    virtual size_t              countNeighborMines(size_t id) const;
    virtual std::vector<size_t> neighborIds(size_t id) const = 0;
    virtual void                openAdjacentCells(Cell* cell);
    virtual void                initialize(size_t cells_counter);
    virtual void                randomize();

protected: // data
    size_t            height_ = 0;
    size_t            width_  = 0;
    size_t            flags_  = 0;
    std::vector<Cell> cells_;
    BoardState        board_state_;
};

#endif // STANDARDBOARD_HPP
