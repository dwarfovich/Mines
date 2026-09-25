#pragma once

#include <QString>

namespace constants {
namespace graph_board {

inline constexpr std::size_t max_attempts_to_find_neighbor = 50;

inline const QString    sprites_path = QStringLiteral(":/gfx/cells_round.png");
inline constexpr int    node_z_value = 1;
inline constexpr int    edge_z_value = 1;

inline constexpr qreal  scene_update_delay = 1000. / 66.;
//inline constexpr double user_speed_conversion_coefficient = 2. / 40.;
//inline constexpr double random_angle_range = 0.4;


}  // namespace graph_board
}  // namespace constants