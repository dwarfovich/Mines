#ifndef BOARDSTATE_HPP
#define BOARDSTATE_HPP

#include "game_state.hpp"

struct BoardState
{
    int mines = 0;
    int empty_cells = 0;
    int flags = 0;
    bool first_cell_opened = false;
    GameState game_state = GameState::Playing;
};

#endif // BOARDSTATE_HPP
