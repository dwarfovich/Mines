#pragma once

#include <QString>

namespace constants {
namespace graph_board {

inline constexpr std::size_t max_attempts_to_find_neighbor = 30;

inline const QString    sprites_path = QStringLiteral(":/gfx/cells_round.png");
inline constexpr int    node_z_value = 1;
inline constexpr int    edge_z_value = 1;

}  // namespace graph_board
}  // namespace constants