#ifndef CELL_HPP
#define CELL_HPP

class Cell
{
public:
    virtual ~Cell() = default;

    bool   is_closed : 1  = true;
    bool   has_flag : 1   = false;
    bool   has_mine : 1   = false;
    size_t neighbor_mines = 0;
    size_t id             = 0;
};

#endif // CELL_HPP
