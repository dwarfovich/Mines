#include "triangulator.hpp"
#include "triangulator.hpp"
#include "qpointf_hasher.hpp"

#include <unordered_map>
#include <unordered_set>

void Triangulator::triangulate(const std::vector<QPointF> &points)
{
    clear();
    if (points.size() < 3) {
        return;
    }

    // Get supertriangle
    Triangle super_triangle = superTriangle(points);
    std::list<Edge> polygon;
    triangulation_.push_back(super_triangle);
    // Main loop
    std::list<Triangle> bad_triangles;
    for (size_t n = 0; n < points.size(); ++n) {
        const QPointF& point = points[n];
        bad_triangles.clear();
        // Find bad triangles for new point
        for (const auto& triangle : triangulation_) {
            if (triangle.circumcircleContains(point)) {
                bad_triangles.push_back(triangle);
            }
        }
        // Find boundary polygon for given point
        polygon = getBoundaryPolygon(bad_triangles);
        // Remove bad triangles from triangulation
        for (const auto& bad_triangle : bad_triangles){
            triangulation_.remove(bad_triangle);
        }
        // Re-triangulate the polygonal hole
        for (const auto& edge : polygon){
            triangulation_.push_back({point, edge.point1, edge.point2});
        }
    }
    // Remove super-triangle & node-associated triangles from triangulation
    cleanTriangulation(super_triangle);
    formEdges(points);
}

Triangle Triangulator::superTriangle(const std::vector<QPointF> &points) const
{
    // TODO: Check if points are collinear
    auto x_minmax = minmax_element(points.cbegin(), points.cend(),
                                   [](const QPointF& a, const QPointF& b) { return a.x() < b.x(); });
    auto y_minmax = minmax_element(points.cbegin(), points.cend(),
                                   [](const QPointF& a, const QPointF& b) { return a.y() < b.y(); });
    double w = x_minmax.second->x() - x_minmax.first->x(); // Bounding rect width
    double h = y_minmax.second->y() - y_minmax.first->y(); // Bounding rect height
    double h_mid = (y_minmax.first->y() + y_minmax.second->y()) / 2.; // Bounding rect middle y
    const double offset = 1.;
    return { {x_minmax.first->x() - offset, h_mid - 1.5*h - offset},
        {x_minmax.first->x() - offset, h_mid + 1.5*h + offset},
        {x_minmax.first->x() + 2*w, h_mid} };
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
    size_t counter1 = 0;
    for (const auto& bad_triangle : bad_triangles) {
        const auto& edges = bad_triangle.edges() ;
        for (const auto& edge : edges) {
            is_shared_edge = false;
            size_t counter2 = 0;
            for(const auto& bad_triangle2 : bad_triangles) {
                if (counter1 != counter2 && bad_triangle2.contains(edge)) {
                    is_shared_edge = true;
                    break;
                }
                ++counter2;
            }
            if (!is_shared_edge) {
                polygon.push_back(edge);
            }
        }
        ++counter1;
    }
    return polygon;
}

void Triangulator::cleanTriangulation(const Triangle& super_triangle)
{
    auto iter = triangulation_.cbegin();
    while (iter != triangulation_.cend()) {
        const auto& triangle = *iter;
        if ( triangle.contains(super_triangle[0])
             || triangle.contains(super_triangle[1])
             || triangle.contains(super_triangle[2]) )
        {
            iter = triangulation_.erase(iter);
        }
        else {
            ++iter;
        }
    }
}

void Triangulator::formEdges(const std::vector<QPointF>& points)
{
    std::unordered_map<QPointF, size_t, QPointFHasher> map;
    for (size_t i = 0; i < points.size(); ++i) {
        map[points[i]] = i;
    }
    std::vector<std::unordered_set<size_t>> temp_edges_ (points.size());
    for (const auto& triangle : triangulation_) {
        const auto& vertices = triangle.vertices();
        size_t point1 = map[vertices[0]];
        size_t point2 = map[vertices[1]];
        size_t point3 = map[vertices[2]];
        temp_edges_[point1].insert(point2);
        temp_edges_[point1].insert(point3);
        temp_edges_[point2].insert(point1);
        temp_edges_[point2].insert(point3);
        temp_edges_[point3].insert(point1);
        temp_edges_[point3].insert(point2);
    }
    edges_.resize(points.size());
    for (size_t i = 0; i < temp_edges_.size(); ++i) {
        edges_[i].insert(edges_[i].cend(), temp_edges_[i].cbegin(), temp_edges_[i].cend());
    }
}
