#pragma once

#include <QString>

namespace constants {
namespace polyomino_board {

inline const QString    sprites_path    = QStringLiteral(":/gfx/transparent_images.png");
inline constexpr int    sub_cell_size   = 64;
inline constexpr int    sprite_size     = 64;
inline constexpr size_t empty_matrix_id = -1;

} // namespace polyomino_board
} // namespace constants