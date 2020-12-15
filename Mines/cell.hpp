#ifndef CELL_HPP
#define CELL_HPP

class Cell
{
public:
    bool is_closed : 1 = true;
    bool hasFlag   : 1 = false;
    bool hasMine   : 1 = false;
    int  neighbor_mines = 0;
    int id = -1;
};

#endif // CELL_HPP
