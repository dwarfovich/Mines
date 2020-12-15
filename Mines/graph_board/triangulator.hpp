#ifndef TRIANGULATOR_HPP
#define TRIANGULATOR_HPP

#include "edge.hpp"
#include "triangle.hpp"

#include <list>
#include <vector>

using EdgeVector = std::vector<std::vector<size_t>>;

class Triangulator
{
public:


    virtual void triangulate (const std::vector<QPointF>& points);
    virtual Triangle superTriangle(const std::vector<QPointF>& points) const;
    virtual EdgeVector getEdges ();
    virtual void clear ();

private: // methods
    std::list<Edge> getBoundaryPolygon (std::list<Triangle>& bad_triangles);
    void cleanTriangulation (const Triangle& super_triangle);
    void formEdges (const std::vector<QPointF>& points);

private:
    std::list<Triangle> triangulation_;
    EdgeVector edges_;
};

#endif // TRIANGULATOR_HPP
