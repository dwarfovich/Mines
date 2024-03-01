#pragma once

class GraphBoardParameters
{
public:
    std::size_t nodes_count          = 0;
    std::size_t mines_count          = 0;
    std::size_t maximum_neighbors    = 1;
    bool   allow_disjoint_graph = false;
    qreal  speed                = 0;
};