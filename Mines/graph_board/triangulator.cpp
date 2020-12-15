#include "triangulator.hpp"
#include "qpointf_hasher.hpp"

#include <unordered_map>
#include <unordered_set>

void Triangulator::triangulate(const std::vector<QPointF> &points)
{
    clear();
    if (points.size() < 3)
        return;

    // Get supertriangle
    Triangle super_triangle {superTriangle(points)};
    std::list<Edge> polygon;
    triangulation_.push_back(super_triangle);
    // Main loop
    std::list<Triangle> bad_triangles;
    for (size_t n = 0; n < points.size(); ++n) {
        const QPointF& point = points[n];
        bad_triangles.clear();
        // Find bad triangles for new point
        for (const auto& triangle : triangulation_)
            if (triangle.circumcircleContains(point))
                bad_triangles.push_back(triangle);
        // Find boundary polygon for given point
        polygon = getBoundaryPolygon(bad_triangles);
        // Remove bad triangles from triangulation
        for (const auto& bad_triangle : bad_triangles)
            triangulation_.remove(bad_triangle);
        // Re-triangulate the polygonal hole
        for (const auto& edge : polygon)
            triangulation_.push_back({point, edge.point1, edge.point2});
    }
    // Remove super-triangle & node-associated triangles from triangulation
    cleanTriangulation(super_triangle);
    formEdges(points);
}

Triangle Triangulator::superTriangle(const std::vector<QPointF> &points) const
{
    size_t points_number = points.size();
    if (points_number == 0) {
        return { {0., 0.}, {0., 0.}, {0., 0.} };
    } else if (points_number == 1) {
        double offset = 100.;
        return { {points[0].x() - offset, points[0].y() - offset},
            {points[0].x() + offset, points[0].y() - offset},
            {points[0].x(),          points[0].y() + offset} };
    }
    // If points_number > 1
    // TODO: Check if points are not collinear
    auto x_minmax = minmax_element(points.begin(), points.end(),
                                   [](const QPointF& a, const QPointF& b) { return a.x() < b.x(); });
    auto y_minmax = minmax_element(points.begin(), points.end(),
                                   [](const QPointF& a, const QPointF& b) { return a.y() < b.y(); });
    double w = x_minmax.second->x() - x_minmax.first->x();         // Bounding rect width
    double h = y_minmax.second->y() - y_minmax.first->y();         // Bounding rect height
    double h0 = (y_minmax.first->y() + y_minmax.second->y()) / 2.; // Bounding rect middle y
    const double offset = 999999.; // NOTE: Large offset help with bad triangles
    return { {x_minmax.first->x() - offset, h0 - 1.5*h - offset},
        {x_minmax.first->x() - offset, h0 + 1.5*h + offset},
        {x_minmax.first->x() + 2*w, h0} };
}

EdgeVector Triangulator::getEdges()
{
    return edges_;
}

void Triangulator::clear()
{
    triangulation_.clear();
    edges_.clear();
}

std::list<Edge> Triangulator::getBoundaryPolygon(std::list<Triangle>& bad_triangles)
{
    // TODO: Reimplement this function
    bool is_shared_edge;
    std::list<Edge> polygon;
    size_t c1 = 0;
    for (const auto& bad_triangle : bad_triangles) {
        const auto& edges = bad_triangle.edges() ;
        for (const auto& edge : edges) {
            is_shared_edge = false;
            size_t c2 = 0;
            for(const auto& bt2 : bad_triangles) {
                if (c1 != c2 && bt2.contains(edge)) {
                    is_shared_edge = true;
                    break;
                }
                ++c2;
            }
            if (!is_shared_edge)
                polygon.push_back(edge);
        }
        ++c1;
    }
    return polygon;
}

void Triangulator::cleanTriangulation(const Triangle& super_triangle)
{
    auto iter = triangulation_.begin();
    while (iter != triangulation_.end()) {
        auto& triangle = *iter;
        if (      triangle.contains(super_triangle[0])
                  || triangle.contains(super_triangle[1])
                  || triangle.contains(super_triangle[2]) )
        {
            iter = triangulation_.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void Triangulator::formEdges(const std::vector<QPointF>& points)
{
    // TODO: Reimplement this function
    std::unordered_map<QPointF, size_t, QPointFHasher> m;
    for (size_t i = 0; i < points.size(); ++i)
        m[points[i]] = i;
    std::vector<std::unordered_set<size_t>> temp_edges_ (points.size());
    for (auto& t : triangulation_) {
        const auto& vertices = t.vertices();
        size_t n1 = m[vertices[0]];
        size_t n2 = m[vertices[1]];
        size_t n3 = m[vertices[2]];
        temp_edges_[n1].insert(n2);
        temp_edges_[n1].insert(n3);
        temp_edges_[n2].insert(n1);
        temp_edges_[n2].insert(n3);
        temp_edges_[n3].insert(n1);
        temp_edges_[n3].insert(n2);
    }
    edges_.resize(points.size());
    for (size_t i = 0; i < temp_edges_.size(); ++i) {
        edges_[i].resize(temp_edges_[i].size());
        size_t j = 0;
        for (const auto& e : temp_edges_[i]) {
            edges_[i][j++] = e;
        }
    }
}
