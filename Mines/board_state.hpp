#ifndef BOARDSTATE_HPP
#define BOARDSTATE_HPP

#include "game_state.hpp"

struct BoardState
{
    size_t mines = 0;
    size_t empty_cells = 0;
    bool first_cell_opened = false;
    GameState game_state = GameState::Playing;
};

#endif // BOARDSTATE_HPP
