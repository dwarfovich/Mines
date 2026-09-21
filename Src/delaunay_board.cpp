#include "delaunay_board.hpp"
#include "qpointf_hasher.hpp"
#include "cell.hpp"
#include "graph_cell_item.hpp"
#include "delaunay_parameters_widget.hpp"
#include "edge_item.hpp"
#include "graph_boards_constants.hpp"
#include "gui/board_scene.hpp"

#include <random>
#include <unordered_map>
#include <unordered_set>

const QString &DelaunayBoard::id() const
{
    static const QString id { QStringLiteral("Delaunay") };
    return id;
}

const QString &DelaunayBoard::name() const
{
    static const QString name = QStringLiteral("Triangulated");
    return name;
}

QWidget *DelaunayBoard::parametersWidget() const
{
    if (!parameters_widget_) {
        parameters_widget_ = new DelaunayParametersWidget { &dummy_parent_widget_ };
    }

    return parameters_widget_;
}

void DelaunayBoard::generate()
{
    if (!parameters_widget_) {
        Q_ASSERT(false);
        return;
    }

    setupParameters();

    board_state_             = {};
    flags_                   = 0;
    board_state_.mines       = parameters_.mines_count;
    board_state_.empty_cells = parameters_.nodes_count - board_state_.mines;

    board_state_.empty_cells = parameters_.nodes_count - board_state_.mines;

    initializeCells(parameters_.nodes_count);
    randomize();

    generatePoints();
    triangulator_.triangulate(points_, bounding_rect_);
    formNeighbors(triangulator_);
    triangulator_.clear();
    board_state_.game_state = GameState::Playing;
}

void DelaunayBoard::formNeighbors(const Triangulator &triangulator)
{
    std::unordered_map<QPointF, std::size_t, QPointFHasher> map;
    for (std::size_t i = 0; i < points_.size(); ++i) {
        map[points_[i]] = i;
    }
    std::vector<std::unordered_set<std::size_t>> temp_edges_(points_.size());
    for (const auto &triangle : triangulator.triangulation()) {
        const auto &vertices = triangle.vertices();
        std::size_t      point1   = map[vertices[0]];
        std::size_t      point2   = map[vertices[1]];
        std::size_t      point3   = map[vertices[2]];
        temp_edges_[point1].insert(point2);
        temp_edges_[point1].insert(point3);
        temp_edges_[point2].insert(point1);
        temp_edges_[point2].insert(point3);
        temp_edges_[point3].insert(point1);
        temp_edges_[point3].insert(point2);
    }

    neighbors_.clear();
    neighbors_.resize(points_.size());
    for (std::size_t i = 0; i < temp_edges_.size(); ++i) {
        neighbors_[i].insert(neighbors_[i].cend(), temp_edges_[i].cbegin(), temp_edges_[i].cend());
    }
}

void DelaunayBoard::setupParameters()
{
    parameters_.nodes_count          = parameters_widget_->nodesCount();
    parameters_.mines_count          = parameters_widget_->minesCount();
}

std::vector<std::size_t> DelaunayBoard::neighborIds(std::size_t id) const
{
    return neighbors_[id];
}
