#pragma once

#include <QString>

namespace constants {
namespace graph_board {

inline const QString    sprites_path                      = QStringLiteral(":/gfx/cells_round.png");
inline constexpr int    node_z_value                      = 1;
inline constexpr int    edge_z_value                      = 1;
inline constexpr size_t grid_step                         = 640.;
inline constexpr size_t max_attempts_to_find_neighbor     = 50;
inline constexpr double random_points_bounding_side       = 1000.;
inline constexpr double bounding_side_adjustment          = random_points_bounding_side / 5.;
inline constexpr double user_speed_conversion_coefficient = 2. / 30.;
inline constexpr double random_angle_range                = 0.4;

} // namespace graph_board
} // namespace constants