#include "triangulator.hpp"
#include "mec.hpp"
#include "triangulator.hpp"
#include "utils.hpp"

#include <algorithm>
#include <unordered_map>



const std::vector<Triangle>& Triangulator::triangulation() const
{
    return triangulation_;
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
                                            return triangle.has(super_triangle[0]) || triangle.has(super_triangle[1]) ||
                                                   triangle.has(super_triangle[2]);
                                        }),
                         triangulation_.end());
}
