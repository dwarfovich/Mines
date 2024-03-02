#ifndef CELL_HPP
#define CELL_HPP

#include <cstddef>

class Cell
{
public:
    Cell() = default;
    Cell(std::size_t new_id);
    virtual ~Cell() = default;

    bool   is_closed : 1  = true;
    bool   has_flag : 1   = false;
    bool   has_mine : 1   = false;
    std::size_t neighbor_mines = 0;
    std::size_t id             = 0;
};

#endif // CELL_HPP
