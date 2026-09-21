#pragma once

#include <QString>
#include <QPen>

namespace constants {
namespace polyomino_board {

inline const QString    sprites_path    = QStringLiteral(":/gfx/transparent_images.png");
inline constexpr int    sub_cell_size   = 64;
inline constexpr int    sprite_size     = 64;
inline constexpr size_t empty_matrix_id = -1;
inline constexpr size_t hue             = 240;
inline constexpr size_t min_saturation  = 60;
inline constexpr size_t max_saturation  = 100;
inline constexpr size_t min_color_value = 220;
inline constexpr size_t max_color_value = 255;
inline constexpr int    font_size       = 30;
inline const QPen       border_pen      = QColor { Qt::black };
inline const QBrush     opened_brush    = QColor { 230, 255, 200 };
inline const QBrush     hovered_brush   = QColor { 220, 220, 255 };

} // namespace polyomino_board
} // namespace constants