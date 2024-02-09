#include "polymino_cell_item.hpp"
#include "polymino_cell.hpp"
#include "direction.hpp"
#include "utils.hpp"
#include "qpoint_hasher.hpp"

#include <QPainter>
#include <QGraphicsRectItem>

#include <unordered_set>

void PolyminoCellItem::setSize(int size)
{
    size_ = size;
}

QPainterPath PolyminoCellItem::shape() const
{
    return painter_path_;
}

QRectF PolyminoCellItem::boundingRect() const
{
    return bounding_rect_;
}

void PolyminoCellItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush({ color_ });
    painter->drawPolygon(polygon_);

    painter->drawPixmap(cell_description_rect_, *sprites_, spriteRect(CellState(1)));
    //painter->drawPixmap(boundingRect(), sprites_, spriteRect(cellState()));
    /*void PaintFlag(QPainter * painter);
    void PaintOpenedMine(QPainter * painter);
    void PaintMissedFlag(QPainter * painter);
    void PaintMissedMine(QPainter * painter);*/

    //if (!cell_->is_closed) {
    //    QFontMetrics fm(painter->font());
    //    auto         xoffset = fm.boundingRect(QString::number(cell_->neighbor_mines)).width();
    //    auto         yoffset = fm.boundingRect(QString::number(cell_->neighbor_mines)).height();
    //    painter->drawText((cell_description_rect_.width() - xoffset) / 2,
    //                      (cell_description_rect_.height() + yoffset) / 2,
    //                      QString::number(cell_->neighbor_mines));
    //    // painter->drawText(cell_description_rect_, QString::number(cell_->neighbor_mines));
    //}
}

void PolyminoCellItem::initialize(PolyminoCell* cell, const QColor& color)
{
    if (!sprites_) {
        sprites_ = std::make_unique<QPixmap>(":/gfx/transparent_images.png");
    }
    setCell(cell);
    color_ = color;

    setPos(cell->center.x() * size_, cell->center.y() * size_);

    painter_path_          = createPainterPath(*cell);
    polygon_               = painter_path_.toFillPolygon().toPolygon();
    bounding_rect_         = painter_path_.boundingRect();
    cell_description_rect_ = findCellDescriptionRect(*cell);
}

QPainterPath PolyminoCellItem::createPainterPath(const PolyminoCell& cell) const
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
                    r = lines.insert(
                        { { shift.x() * size_, shift.y() * size_ }, { shift.x() * size_ + size_, shift.y() * size_ } });
                    break;
                case Direction::Right:
                    r = lines.insert({ { shift.x() * size_ + size_, shift.y() * size_ },
                                       { shift.x() * size_ + size_, shift.y() * size_ + size_ } });
                    break;
                case Direction::Down:
                    r = lines.insert({ { shift.x() * size_ + size_, shift.y() * size_ + size_ },
                                       { shift.x() * size_, shift.y() * size_ + size_ } });
                    break;
                case Direction::Left:
                    r = lines.insert(
                        { { shift.x() * size_, shift.y() * size_ + size_ }, { shift.x() * size_, shift.y() * size_ } });
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

QRect PolyminoCellItem::findCellDescriptionRect(const PolyminoCell& cell) const
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
        return { 0, 0, size_, size_ };
    }

    if (::contains(cell.shifts, QPoint { mid_x, mid_y })) {
        return { mid_x * size_, mid_y * size_, size_, size_ };
    }

    const QPointF midShift { static_cast<qreal>(mid_x), static_cast<qreal>(mid_y) };
    if (midShift == QPointF { 0., 0. }) {
        return { 0, 0, size_, size_ };
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

    return { min_shift.x() * size_, min_shift.y() * size_, size_, size_ };
}