#ifndef ID_BASED_BOARD_HPP
#define ID_BASED_BOARD_HPP

#include "board.hpp"
#include "cell.hpp"

class RectangleBoardParametersWidget;

class IdBasedBoard : public Board
{
    Q_OBJECT

public:
    size_t      flags() const override;
    const Cell* cellById(size_t id) const override;
    void        openCell(size_t id) override;
    void        toggleFlag(size_t id) override;

protected: // methods
    virtual std::vector<size_t> neighborIds(size_t id) const = 0;
    virtual Cell*               cellById(size_t id);
    virtual void                relocateMine(Cell* cell);
    virtual void                reveal();
    virtual size_t              countNeighborMines(size_t id) const;
    virtual void                openAdjacentCells(Cell* cell);
    virtual void                initialize(size_t cells_counter);
    virtual void                randomize();

protected: // data
    size_t            flags_ = 0;
    std::vector<Cell> cells_;
};

#endif // ID_BASED_BOARD_HPP
