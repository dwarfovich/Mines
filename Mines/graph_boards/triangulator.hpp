#ifndef TRIANGULATOR_HPP
#define TRIANGULATOR_HPP

#include "edge.hpp"
#include "triangle.hpp"

#include <QRectF>

#include <vector>

class Triangulator
{
public:
    void                         triangulate(const std::vector<QPointF>& points, const QRectF& bounding_rect = {});
    const std::vector<Triangle>& triangulation() const;
    Triangle superTriangle(const std::vector<QPointF>& points, const QRectF& bounding_rect = {}) const;
    void     clear();

private: // methods
    void cleanTriangulation(const Triangle& super_triangle);

private: // data
    std::vector<Triangle> triangulation_;
};

#endif // TRIANGULATOR_HPP
