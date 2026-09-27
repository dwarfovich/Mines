#pragma once

#include "edge.hpp"
#include "triangle.hpp"
#include "mec.hpp"

#include <QRectF>

#include <vector>

class Triangulator {
public:
    template <typename CellType>
    void                         triangulate(const std::vector<CellType>& cells, const QRectF& bounding_rect = {});
    const std::vector<Triangle>& triangulation() const;
    void                         clear();

private:  // methods
    template <typename CellType>
    Triangle superTriangle(const std::vector<CellType>& cells, const QRectF& bounding_rect = {}) const;
    void     cleanTriangulation(const Triangle& super_triangle);

private:  // data
    std::vector<Triangle> triangulation_;
};

template <typename CellType>
void Triangulator::triangulate(const std::vector<CellType>& cells, const QRectF& bounding_rect)
{
    clear();
    if (cells.size() < 3) {
        return;
    }

    const auto& super_triangle = superTriangle(cells, bounding_rect);
    triangulation_.push_back(super_triangle);

    for (const auto& cell : cells) {
        std::vector<Triangle>                             temp;
        std::unordered_map<Edge, std::size_t, EdgeHasher> edges;
        for (const auto& triangle : triangulation_) {
            if (triangle.circumcircleContains(cell.coordinates)) {
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
                temp.push_back({cell.coordinates, edge[0], edge[1]});
            }
        }

        triangulation_ = std::move(temp);
    }

    cleanTriangulation(super_triangle);
}

template <typename CellType>
Triangle Triangulator::superTriangle(const std::vector<CellType>& cells, const QRectF& bounding_rect) const
{
    if (bounding_rect.isValid()) {
        static const double magicEnlargement = 100'000;
        double              mid_x = (bounding_rect.right() + bounding_rect.left()) / 2.;
        double              mid_y = (bounding_rect.bottom() + bounding_rect.top()) / 2.;
        double              min_y = bounding_rect.top() - bounding_rect.height() * 2. - magicEnlargement;
        double              max_y = bounding_rect.bottom() + bounding_rect.height() + magicEnlargement;
        double              min_x = bounding_rect.left() - bounding_rect.width() - magicEnlargement;
        double              max_x = bounding_rect.right() + bounding_rect.width() + magicEnlargement;
        return {
            {mid_x, min_y},
            {min_x, max_y},
            {max_x, max_y},
        };
    } else {
        auto                    mec = minimalEnclosingCircle(cells);
        static constexpr double magicEnlargement = 1'000'000.;
        mec.radius += magicEnlargement;
        const auto x = std::sqrt(sqr(mec.radius * 2) - sqr(mec.radius));

        return {
            {mec.center.x(), mec.center.y() + 2 * mec.radius},
            {mec.center.x() - x, mec.center.y() - mec.radius},
            {mec.center.x() + x, mec.center.y() - mec.radius},
        };
    }
}