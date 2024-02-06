#include "polymino_cell_item.hpp"
#include "polymino_cell.hpp"
#include "direction.hpp"
#include "utils.hpp"
#include "qpoint_hasher.hpp"

#include <QPainter>
#include <QGraphicsRectItem>

#include <unordered_set>

int PolyminoCellItem::size_ = 0;

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
    //painter->drawRect(0, 0, 10, 10);
}

void PolyminoCellItem::initialize(PolyminoCell* cell, const QColor& color)
{
    setCell(cell);
    color_ = color;

    setPos(cell->center.x() * size_, cell->center.y() * size_);

    painter_path_ = createPainterPath(*cell);
    polygon_      = painter_path_.toFillPolygon().toPolygon();
    bounding_rect_ = painter_path_.boundingRect();
}

QPainterPath PolyminoCellItem::createPainterPath(const PolyminoCell& cell) const
{
    QPainterPath path;
    std::unordered_map<QPoint, QPoint, QPointHasher> lines;
    for (const auto& shift : cell.shifts) {
        Q_ASSERT(!cell.shifts.empty());
        qDebug() << "shift: " <<  shift;
        for (const auto& direction : directionsArray) {
            const auto& neighborShift = shift + directionToShift(direction);
            if (::contains(cell.shifts, neighborShift)) {
                continue;
            }

            qDebug() << neighborShift;

            using LinesIterator = std::unordered_map<QPoint, QPoint, QPointHasher>::iterator;
            std::pair<LinesIterator, bool> r;
            switch (direction) {
                case Direction::Up:
                    r = lines.insert(
                        { { shift.x() * size_, shift.y() * size_ }, { shift.x() * size_ + size_, shift.y() * size_ } });
                    break;
                case Direction::Right:
                    r = lines.insert(
                        { { shift.x() * size_ + size_, shift.y() * size_ }, { shift.x() * size_ + size_, shift.y() * size_ + size_} });
                    break;
                case Direction::Down:
                    r = lines.insert({ { shift.x() * size_ + size_, shift.y() * size_ + size_ },
                                   { shift.x() * size_, shift.y() * size_ + size_ } });
                    break;
                case Direction::Left:
                    r = lines.insert({ { shift.x() * size_, shift.y() * size_ + size_ },
                                   { shift.x() * size_, shift.y() * size_} });
                    break;
                default: break;
            }
            Q_ASSERT(r.second);
        }
    }
    int counter = 0;
    QPoint p1 = lines.begin()->first;
    path.moveTo(p1);
    auto p2 = lines.begin()->second;
    path.lineTo(p2);
    while (counter < lines.size()) {
        p1 = p2;
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
