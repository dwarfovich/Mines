#include "triangulator.hpp"
#include "triangulator.hpp"
#include "mec.hpp"
#include "utils.hpp"

#include <algorithm>
#include <unordered_map>

void Triangulator::triangulate(const std::vector<QPointF>& points, const QRectF& bounding_rect)
{
    clear();
    if (points.size() < 3) {
        return;
    }

    const auto& super_triangle = superTriangle(points, bounding_rect);
    triangulation_.push_back(super_triangle);

    for (const auto& point : points) {
        std::vector<Triangle>                        temp;
        std::unordered_map<Edge, std::size_t, EdgeHasher> edges;
        for (const auto& triangle : triangulation_) {
            if (triangle.circumcircleContains(point)) {
                const auto& bad_edges = triangle.edges();
                ++edges[bad_edges[0]];
                ++edges[bad_edges[1]];
                ++edges[bad_edges[2]];
            } else {
                temp.push_back(triangle);
            }
        }

        for (const auto& [edge, count] : edges) {
            if (count == 1) {
                temp.push_back({ point, edge[0], edge[1] });
            }
        }

        triangulation_ = std::move(temp);
    }

    cleanTriangulation(super_triangle);
}

const std::vector<Triangle>& Triangulator::triangulation() const
{
    return triangulation_;
}

Triangle Triangulator::superTriangle(const std::vector<QPointF>& points, const QRectF& bounding_rect) const
{
    if (bounding_rect.isValid()) {
        static const double magicEnlargement = 100'000;
        double              mid_x            = (bounding_rect.right() + bounding_rect.left()) / 2.;
        double              mid_y            = (bounding_rect.bottom() + bounding_rect.top()) / 2.;
        double              min_y            = bounding_rect.top() - bounding_rect.height() * 2. - magicEnlargement;
        double              max_y            = bounding_rect.bottom() + bounding_rect.height() + magicEnlargement;
        double              min_x            = bounding_rect.left() - bounding_rect.width() - magicEnlargement;
        double              max_x            = bounding_rect.right() + bounding_rect.width() + magicEnlargement;
        return {
            { mid_x, min_y },
            { min_x, max_y },
            { max_x, max_y },
        };
    } else {
        auto                    mec              = minimalEnclosingCircle(points);
        static constexpr double magicEnlargement = 1'000'000.;
        mec.radius += magicEnlargement;
        const auto x = std::sqrt(sqr(mec.radius * 2) - sqr(mec.radius));

        return {
            { mec.center.x(), mec.center.y() + 2 * mec.radius },
            { mec.center.x() - x, mec.center.y() - mec.radius },
            { mec.center.x() + x, mec.center.y() - mec.radius },
        };
    }
}

void Triangulator::clear()
{
    triangulation_.clear();
}

void Triangulator::cleanTriangulation(const Triangle& super_triangle)
{
    triangulation_.erase(std::remove_if(triangulation_.begin(),
                                        triangulation_.end(),
                                        [&super_triangle](const auto& triangle) {
                                            return triangle.has(super_triangle[0]) || triangle.has(super_triangle[1])
                                                   || triangle.has(super_triangle[2]);
                                        }),
                         triangulation_.end());
}
