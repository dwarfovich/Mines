#pragma once

#include <QString>

namespace constants {
namespace graph_board {

inline const QString    sprites_path                  = QStringLiteral(":/gfx/cells_round.png");
inline constexpr size_t grid_step                     = 640.;
inline constexpr size_t max_attempts_to_find_neighbor = 50;
inline constexpr double random_points_bounding_side   = 1000.;
inline constexpr double bounding_side_adjustment      = random_points_bounding_side / 5.;

} // namespace graph_board
} // namespace constants