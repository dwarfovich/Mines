#include "polyomino_cell_item.hpp"
#include "polyomino_cell.hpp"
#include "polyomino_board_constants.hpp"
#include "direction.hpp"
#include "utils.hpp"
#include "qpoint_hasher.hpp"

#include <QPoint>
#include <QPainter>
#include <QGraphicsRectItem>

#include <unordered_set>

QPainterPath PolyominoCellItem::shape() const
{
    return painter_path_;
}

QRectF PolyominoCellItem::boundingRect() const
{
    return bounding_rect_;
}

void PolyominoCellItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(constants::polyomino_board::border_pen);

    if (cell_->is_closed) {
        if (IsHovered()) {
            painter->setBrush(constants::polyomino_board::hovered_brush);
        } else {
            painter->setBrush(closed_brush_);
        }
        painter->drawPolygon(polygon_);
    } else {
        painter->setBrush(constants::polyomino_board::opened_brush);
        painter->drawPolygon(polygon_);
    }
    const auto& rect = spriteRect(cellState());
    if (rect.isNull()) {
        if (!cell_->is_closed) {
            paintMinesCount(painter);
        }
    } else {
        painter->drawPixmap(cell_info_rect_, *sprites_, rect);
    }
}

void PolyominoCellItem::initialize(PolyominoCell* cell, const QColor& color)
{
    if (!sprites_) {
        sprites_ = std::make_unique<QPixmap>(":/gfx/transparent_images.png");
    }
    setCell(cell);
    closed_brush_ = { color };

    using namespace constants::polyomino_board;
    setPos(cell->center.x() * sub_cell_size, cell->center.y() * sub_cell_size);

    painter_path_   = createPainterPath(*cell);
    polygon_        = painter_path_.toFillPolygon().toPolygon();
    bounding_rect_  = painter_path_.boundingRect();
    cell_info_rect_ = findCellDescriptionRect(*cell);
}

QPainterPath PolyominoCellItem::createPainterPath(const PolyominoCell& cell) const
{
    Q_ASSERT(!cell.shifts.empty());

    std::unordered_map<QPoint, QPoint, QPointHasher> lines;
    for (const auto& shift : cell.shifts) {
        for (const auto& direction : directions_array) {
            const auto& neighborShift = shift + directionToShift(direction);
            if (::contains(cell.shifts, neighborShift)) {
                continue;
            }

            using namespace constants::polyomino_board;
            using LinesIterator = std::unordered_map<QPoint, QPoint, QPointHasher>::iterator;
            std::pair<LinesIterator, bool> r;
            const int                      x = shift.x() * sub_cell_size;
            const int                      y = shift.y() * sub_cell_size;
            switch (direction) {
                case Direction::Up: r = lines.insert({ { x, y }, { x + sub_cell_size, y } }); break;
                case Direction::Right:
                    r = lines.insert({ { x + sub_cell_size, y }, { x + sub_cell_size, y + sub_cell_size } });
                    break;
                case Direction::Down:
                    r = lines.insert({ { x + sub_cell_size, y + sub_cell_size }, { x, y + sub_cell_size } });
                    break;
                case Direction::Left: r = lines.insert({ { x, y + sub_cell_size }, { x, y } }); break;
                default: break;
            }
            Q_ASSERT(r.second && "Line wasn't inserted, so there is an error");
        }
    }

    QPainterPath path;
    auto         iter        = lines.cbegin();
    const auto   first_point = iter->first;
    path.moveTo(first_point);
    while (iter != lines.cend() && iter->second != first_point) {
        path.lineTo(iter->second);
        iter = lines.find(iter->second);
    }
    path.closeSubpath();

    return path;
}

QRect PolyominoCellItem::findCellDescriptionRect(const PolyominoCell& cell) const
{
    Q_ASSERT(!cell.shifts.empty());

    const auto min_max_x =
        std::minmax_element(std::cbegin(cell.shifts), std::cend(cell.shifts), [](const auto& lhs, const auto& rhs) {
            return lhs.x() < rhs.x();
        });
    const auto min_x = min_max_x.first->x();
    const auto max_x = min_max_x.second->x();
    const auto min_max_y =
        std::minmax_element(std::cbegin(cell.shifts), std::cend(cell.shifts), [](const auto& lhs, const auto& rhs) {
            return lhs.y() < rhs.y();
        });
    const auto min_y = min_max_x.first->y();
    const auto max_y = min_max_x.second->y();

    const auto mid_x = (min_x + max_x) / 2;
    const auto mid_y = (min_y + max_y) / 2;

    using namespace constants::polyomino_board;
    if (mid_x == 0 && mid_y == 0) {
        return { 0, 0, sub_cell_size, sub_cell_size };
    }

    if (::contains(cell.shifts, QPoint { mid_x, mid_y })) {
        return { mid_x * sub_cell_size, mid_y * sub_cell_size, sub_cell_size, sub_cell_size };
    }

    const QPointF midShift { static_cast<qreal>(mid_x), static_cast<qreal>(mid_y) };
    if (midShift == QPointF { 0., 0. }) {
        return { 0, 0, sub_cell_size, sub_cell_size };
    }

    auto min_distance = std::numeric_limits<qreal>::max();
    auto min_shift    = cell.shifts.front();
    for (const auto& shift : cell.shifts) {
        const auto distance = euclideanDistance(midShift, QPointF{shift});
        if (distance < min_distance) {
            min_distance = distance;
            min_shift    = shift;
        }
    }

    return { min_shift.x() * sub_cell_size, min_shift.y() * sub_cell_size, sub_cell_size, sub_cell_size };
}

QRectF PolyominoCellItem::spriteRect(CellState state) const
{
    using namespace constants::polyomino_board;
    if (state == CellState::ClosedWithFlag || state == CellState::OpenedMine || state == CellState::MissedFlag
        || state == CellState::MissedMine) {
        return QRectF { sprite_size * (qreal(state) - 1), 0., qreal(sprite_size), qreal(sprite_size) };
    } else {
        return {};
    }
}

void PolyominoCellItem::paintMinesCount(QPainter* painter)
{
    if (!mines_count_attributes_initialized) {
        initializeMinesCountAttributes(painter);
    }
    if (cell_->neighbor_mines != 0) {
        auto font = painter->font();
        font.setPixelSize(constants::polyomino_board::font_size);
        painter->setFont(font);
        painter->drawText(cell_info_rect_, Qt::AlignCenter, mines_count_);
    }
}

void PolyominoCellItem::initializeMinesCountAttributes(QPainter* painter)
{
    mines_count_                       = QString::number(cell_->neighbor_mines);
    mines_count_attributes_initialized = true;
}
