#include "polyomino_cell_item.hpp"
#include "polyomino_cell.hpp"
#include "direction.hpp"
#include "utils.hpp"
#include "qpoint_hasher.hpp"

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
    //painter->setBrush(opened_brush_);
    //painter->drawPolygon(polygon_);
    //paintMinesCount(painter);

    if (cell_->is_closed) {
        painter->setBrush(closed_brush_);
        painter->drawPolygon(polygon_);
    } else {
        painter->setBrush(opened_brush_);
        painter->drawPolygon(polygon_);
        const auto& rect = spriteRect(cellState());
        if (rect.isNull()) {
            paintMinesCount(painter);
        } else {
            painter->drawPixmap(boundingRect(), *sprites_, spriteRect(cellState()));
        }
    }
}

void PolyominoCellItem::initialize(PolyominoCell* cell, const QColor& color)
{
    if (!sprites_) {
        sprites_ = std::make_unique<QPixmap>(":/gfx/transparent_images.png");
    }
    setCell(cell);
    closed_brush_ = { color };
    setPos(cell->center.x() * sub_cell_size_, cell->center.y() * sub_cell_size_);

    painter_path_          = createPainterPath(*cell);
    polygon_               = painter_path_.toFillPolygon().toPolygon();
    bounding_rect_         = painter_path_.boundingRect();
    cell_description_rect_ = findCellDescriptionRect(*cell);
}

QPainterPath PolyominoCellItem::createPainterPath(const PolyominoCell& cell) const
{
    Q_ASSERT(!cell.shifts.empty());

    QPainterPath                                     path;
    std::unordered_map<QPoint, QPoint, QPointHasher> lines;
    for (const auto& shift : cell.shifts) {
        for (const auto& direction : directions_array) {
            const auto& neighborShift = shift + directionToShift(direction);
            if (::contains(cell.shifts, neighborShift)) {
                continue;
            }

            using LinesIterator = std::unordered_map<QPoint, QPoint, QPointHasher>::iterator;
            std::pair<LinesIterator, bool> r;
            switch (direction) {
                case Direction::Up:
                    r = lines.insert({ { shift.x() * sub_cell_size_, shift.y() * sub_cell_size_ },
                                       { shift.x() * sub_cell_size_ + sub_cell_size_, shift.y() * sub_cell_size_ } });
                    break;
                case Direction::Right:
                    r = lines.insert({ { shift.x() * sub_cell_size_ + sub_cell_size_, shift.y() * sub_cell_size_ },
                                       { shift.x() * sub_cell_size_ + sub_cell_size_,
                                         shift.y() * sub_cell_size_ + sub_cell_size_ } });
                    break;
                case Direction::Down:
                    r = lines.insert(
                        { { shift.x() * sub_cell_size_ + sub_cell_size_, shift.y() * sub_cell_size_ + sub_cell_size_ },
                          { shift.x() * sub_cell_size_, shift.y() * sub_cell_size_ + sub_cell_size_ } });
                    break;
                case Direction::Left:
                    r = lines.insert({ { shift.x() * sub_cell_size_, shift.y() * sub_cell_size_ + sub_cell_size_ },
                                       { shift.x() * sub_cell_size_, shift.y() * sub_cell_size_ } });
                    break;
                default: break;
            }
            Q_ASSERT(r.second && "Line wasn't inserted, so there is an error");
        }
    }
    int    counter = 0;
    QPoint p1      = lines.begin()->first;
    path.moveTo(p1);
    auto p2 = lines.begin()->second;
    path.lineTo(p2);
    while (counter < lines.size()) {
        p1        = p2;
        auto iter = lines.find(p1);
        if (iter == lines.cend()) {
            break;
        } else {
            p2 = iter->second;
            path.lineTo(p2);
        }
        ++counter;
    }

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

    if (mid_x == 0 && mid_y == 0) {
        return { 0, 0, sub_cell_size_, sub_cell_size_ };
    }

    if (::contains(cell.shifts, QPoint { mid_x, mid_y })) {
        return { mid_x * sub_cell_size_, mid_y * sub_cell_size_, sub_cell_size_, sub_cell_size_ };
    }

    const QPointF midShift { static_cast<qreal>(mid_x), static_cast<qreal>(mid_y) };
    if (midShift == QPointF { 0., 0. }) {
        return { 0, 0, sub_cell_size_, sub_cell_size_ };
    }

    auto min_distance = std::numeric_limits<qreal>::max();
    auto min_shift    = cell.shifts.front();
    for (const auto& shift : cell.shifts) {
        const auto distance = euclideanDistance(midShift, shift.toPointF());
        if (distance < min_distance) {
            min_distance = distance;
            min_shift    = shift;
        }
    }

    return { min_shift.x() * sub_cell_size_, min_shift.y() * sub_cell_size_, sub_cell_size_, sub_cell_size_ };
}

QRectF PolyominoCellItem::spriteRect(CellState state) const
{
    if (state == CellState::ClosedWithFlag || state == CellState::OpenedMine || state == CellState::MissedFlag
        || state == CellState::MissedMine) {
        return QRectF { sprite_size_ * (qreal(state) - 1), 0., qreal(sprite_size_), qreal(sprite_size_) };
    } else {
        return {};
    }
}

void PolyominoCellItem::paintMinesCount(QPainter* painter)
{
    if (!mines_count_attributes_initialized) {
        initializeMinesCountAttributes(painter);
    }
    auto  font = painter->font();
    font.setPixelSize(font_size_);
    painter->setFont(font);
    painter->drawText(cell_description_rect_, Qt::AlignCenter, mines_count_);
}

void PolyominoCellItem::initializeMinesCountAttributes(QPainter* painter)
{
    mines_count_ = QString::number(cell_->neighbor_mines);
    mines_count_attributes_initialized = true;
}
