#ifndef STANDARDBOARD_HPP
#define STANDARDBOARD_HPP

#include "iboard.hpp"
#include "cell.hpp"

class StandardBoard : public IBoard
{
    Q_OBJECT

public:
    int flags() const override;
    const Cell* cellById(int id) const override;
    void openCell(int id) override;
    void toggleFlag(int id) override;
    const BoardState &boardState() const override;
    QWidget *createParametersWidget() const override;
    void generate(QWidget *parameters_widget) override;

protected: // methods
    virtual Cell* cellById(int id);
    virtual void relocateMine(Cell* cell);
    virtual void reveal();
    virtual int countNeighborMines(int id) const;
    virtual std::vector<size_t> neighborIds(size_t id) const = 0;
    virtual void openAdjacentCells(Cell* cell);
    virtual void initialize(size_t cells_counter);
    virtual void randomize();

protected: // data
    size_t height_ = 0;
    size_t width_ = 0;
    int flags_ = 0;
    std::vector<Cell> cells_;
    BoardState board_state_;
};

#endif // STANDARDBOARD_HPP
