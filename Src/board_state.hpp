#pragma once

#include "game_state.hpp"

#include <cstddef>

#include <QtCore>

struct BoardState {
    void decreaseEmptyCells(std::size_t count) {
        if (count <= empty_cells){
            empty_cells -= count;
        } else {
            empty_cells = 0;
            Q_ASSERT(false);
        }
    }
    std::size_t mines = 0;
    std::size_t empty_cells = 0;
    bool        first_cell_opened = false;
    GameState   game_state = GameState::Playing;
};
