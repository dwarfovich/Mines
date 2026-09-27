#include "triangulator.hpp"

const std::vector<Triangle>& Triangulator::triangulation() const
{
    return triangulation_;
}

void Triangulator::clear()
{
    triangulation_.clear();
}

Triangle Triangulator::superTriangle(const QRectF& bounding_rect) const
{
    if (!bounding_rect.isValid()) {
        return {};
    }

    const qreal triangle_side = 2.0 * std::sqrt(bounding_rect.width() * bounding_rect.width() +
                                                bounding_rect.height() * bounding_rect.height());
    const qreal triangle_height = triangle_side * std::sqrt(3.0) / 2.0;

    const auto& cx = bounding_rect.center().x();
    const auto& cy = bounding_rect.center().y();

    QPointF a{cx, cy - 2. * triangle_height / 3.};
    QPointF b{cx - triangle_side / 2., cy + triangle_height / 3.};
    QPointF c{cx + triangle_side / 2., cy + triangle_height / 3.};

    return {a, b, c};
}

void Triangulator::cleanTriangulation(const Triangle& super_triangle)
{
    triangulation_.erase(std::remove_if(triangulation_.begin(),
                                        triangulation_.end(),
                                        [&super_triangle](const auto& triangle) {
                                            return triangle.has(super_triangle[0]) || triangle.has(super_triangle[1]) ||
                                                   triangle.has(super_triangle[2]);
                                        }),
                         triangulation_.end());
}
