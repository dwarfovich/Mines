#include "delaunay_board.hpp"
#include "cell.hpp"
#include "delaunay_parameters_widget.hpp"
#include "edge_item.hpp"
#include "graph_boards_constants.hpp"
#include "graph_cell_item.hpp"
#include "gui/board_scene.hpp"
#include "qpointf_hasher.hpp"

#include <random>
#include <unordered_map>
#include <unordered_set>

const QString& DelaunayBoard::id() const
{
    static const QString id{QStringLiteral("Triangulated")};
    return id;
}

const QString& DelaunayBoard::name() const
{
    static const QString name = QStringLiteral("Triangulated");
    return name;
}

void DelaunayBoard::generate()
{
    auto* parameters_widget = parametersWidget();
    if (!parameters_widget) {
        Q_ASSERT(false);
        return;
    }

    setupParameters();

    board_state_ = {};
    flags_ = 0;
    board_state_.mines = parameters_.mines_count;
    board_state_.empty_cells = parameters_.nodes_count - board_state_.mines;

    board_state_.empty_cells = parameters_.nodes_count - board_state_.mines;

    initializeCells(parameters_.nodes_count);
    randomize();

    assignCoordinatesToCells();
    triangulator_.triangulate(cells_, bounding_rect_);
    formNeighbors(triangulator_);
    triangulator_.clear();
    board_state_.game_state = GameState::Playing;
}

void DelaunayBoard::formNeighbors(const Triangulator& triangulator)
{
    std::unordered_map<QPointF, std::size_t, QPointFHasher> indices;
    for (std::size_t i = 0; i < cells_.size(); ++i) {
        indices[cells_[i].coordinates] = i;
    }
    std::vector<std::unordered_set<std::size_t>> temp_neighbors_(cells_.size());
    for (const auto& triangle : triangulator.triangulation()) {
        const auto& vertices = triangle.vertices();

        std::size_t index1 = indices[vertices[0]];
        std::size_t index2 = indices[vertices[1]];
        std::size_t index3 = indices[vertices[2]];
        temp_neighbors_[index1].insert(index2);
        temp_neighbors_[index1].insert(index3);
        temp_neighbors_[index2].insert(index1);
        temp_neighbors_[index2].insert(index3);
        temp_neighbors_[index3].insert(index1);
        temp_neighbors_[index3].insert(index2);
    }

    neighbors_.clear();
    neighbors_.resize(cells_.size());
    for (std::size_t i = 0; i < temp_neighbors_.size(); ++i) {
        neighbors_[i].insert(neighbors_[i].cend(), temp_neighbors_[i].cbegin(), temp_neighbors_[i].cend());
    }
}

void DelaunayBoard::setupParameters()
{
    auto* parameters_widget = parametersWidget();
    if (!parameters_widget) {
        Q_ASSERT(false);
        return;
    }

    parameters_.nodes_count = parameters_widget->nodesCount();
    parameters_.mines_count = parameters_widget->minesCount();
}