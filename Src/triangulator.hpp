#pragma once

#include "edge.hpp"
#include "mec.hpp"
#include "triangle.hpp"

#include <QRectF>

#include <vector>

class Triangulator {
public:
    template <typename CellType>
    void                         triangulate(const std::vector<CellType>& cells, const QRectF& bounding_rect = {});
    const std::vector<Triangle>& triangulation() const;
    void                         clear();

private:  // methods
    Triangle superTriangle(const QRectF& bounding_rect) const;
    void     cleanTriangulation(const Triangle& super_triangle);

private:  // data
    std::vector<Triangle> triangulation_;
};

template <typename CellType>
void Triangulator::triangulate(const std::vector<CellType>& cells, const QRectF& bounding_rect)
{
    // Bowyer–Watson algorithm
    clear();
    if (cells.size() < 3) {
        return;
    }

    const auto& super_triangle = superTriangle(bounding_rect);
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
