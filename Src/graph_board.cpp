#include "graph_board.hpp"
#include "graph_parameters_widget.h"
#include "graph_boards_constants.hpp"
#include "graph_cell_item.hpp"
#include "edge.hpp"
#include "edge_item.hpp"
#include "qpointf_hasher.hpp"
#include "gui/board_scene.hpp"

#include <unordered_set>

const QString &GraphBoard::id() const
{
    static const QString id { QStringLiteral("Graph") };
    return id;
}

const QString &GraphBoard::name() const
{
    static const QString id { QStringLiteral("Graph") };
    return id;
}

QWidget *GraphBoard::parametersWidget() const
{
    if (!parameters_widget_) {
        parameters_widget_ = new GraphParametersWidget { &dummy_parent_widget_ };
    }

    return parameters_widget_;
}

void GraphBoard::generate()
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

    initializeCells(parameters_.nodes_count);
    randomize();

    generatePoints();
    formNeighbors();

    board_state_.game_state = GameState::Playing;
}

void GraphBoard::setupScene(BoardScene *scene)
{
    Q_ASSERT(scene);

    setupCellItems();

    const auto                                  sprite_size = SpriteCellItem::size();
    std::unordered_map<std::size_t, GraphCellItem *> id_to_item_map;
    for (std::size_t id = 0; id < points_.size(); ++id) {
        auto *node_item = new GraphCellItem { cellById(id) };
        node_item->setZValue(constants::graph_board::node_z_value);
        node_item->setPos(points_[id]);
        scene->registerCellItem(node_item);
        id_to_item_map[id] = node_item;
    }

    std::unordered_set<Edge, EdgeHasher> createdEdges;
    for (const auto &[id, item] : id_to_item_map) {
        const auto &neighbors = neighbors_[id];
        const auto &point1    = points_[id];
        for (const auto &buddy_id : neighbors) {
            item->addBuddy(id_to_item_map[buddy_id]);
            const auto &point2 = points_[buddy_id];
            Edge        edge { point1, point2 };
            auto        iter = createdEdges.find(edge);
            if (createdEdges.find(edge) == createdEdges.cend()) {
                createdEdges.insert(edge);
                auto *edge_item = new EdgeItem { edge };
                scene->addItem(edge_item);
                item->addBuddy(edge_item);
                id_to_item_map[buddy_id]->addBuddy(edge_item);
            }
        }
    }

    using namespace constants::graph_board;
    scene->setSceneRect(bounding_rect_.adjusted(
        -bounding_side_adjustment, -bounding_side_adjustment, bounding_side_adjustment, bounding_side_adjustment));
}

std::vector<std::size_t> GraphBoard::neighborIds(std::size_t id) const
{
    return neighbors_[id];
}

void GraphBoard::generatePoints()
{
    bounding_rect_.setLeft(std::numeric_limits<double>::max());
    bounding_rect_.setRight(std::numeric_limits<double>::lowest());
    bounding_rect_.setTop(std::numeric_limits<double>::max());
    bounding_rect_.setBottom(std::numeric_limits<double>::lowest());

    points_.resize(cells_.size());
    std::uniform_real_distribution<> distribution(0, constants::graph_board::random_points_bounding_side);
    for (auto &point : points_) {
        point = { distribution(random_generator_), distribution(random_generator_) };
        bounding_rect_.setLeft(std::min(point.x(), bounding_rect_.left()));
        bounding_rect_.setRight(std::max(point.x(), bounding_rect_.right()));
        bounding_rect_.setTop(std::min(point.y(), bounding_rect_.top()));
        bounding_rect_.setBottom(std::max(point.y(), bounding_rect_.bottom()));
    }
}

void GraphBoard::formNeighbors()
{
    neighbors_.clear();
    neighbors_.resize(points_.size());

    if (!parameters_.allow_disjoint_graph) {
        for (std::size_t i = 1; i < points_.size(); ++i) {
            neighbors_[i].push_back(i - 1);
            neighbors_[i - 1].push_back(i);
        }
    }

    std::uniform_int_distribution<std::size_t> neighbor_distribution(0, points_.size() - 1);
    std::uniform_int_distribution<std::size_t> max_distribution(parameters_.allow_disjoint_graph ? 0 : 1,
                                                           parameters_.maximum_neighbors);
    for (std::size_t i = 0; i < neighbors_.size(); ++i) {
        auto &neighbor_ids  = neighbors_[i];
        const auto  max_neighbors = max_distribution(random_generator_);
        while (neighbor_ids.size() <= max_neighbors) {
            const auto &max_attempts = constants::graph_board::max_attempts_to_find_neighbor;
            auto        neighbor_id  = 0;
            std::size_t      attempt      = 0;
            do {
                neighbor_id = neighbor_distribution(random_generator_);
            } while (((neighbor_id == i) || ::contains(neighbor_ids, neighbor_id)) && (++attempt < max_attempts));
            if (attempt < max_attempts) {
                neighbor_ids.push_back(neighbor_id);
                if (!::contains(neighbors_[neighbor_id], i)) {
                    neighbors_[neighbor_id].push_back(i);
                }
            } else { // Failed to find correct neighbor.
                break;
            }
        }
    }
}

void GraphBoard::setupCellItems()
{
    SpriteCellItem::setSprites(constants::graph_board::sprites_path);
    const auto   sprite_size = SpriteCellItem::size();
    QPainterPath path;
    path.addEllipse(-sprite_size / 2., -sprite_size / 2., sprite_size, sprite_size);
    SpriteCellItem::setShape(path);
}

void GraphBoard::setupParameters()
{
    parameters_.nodes_count          = parameters_widget_->nodesCount();
    parameters_.mines_count          = parameters_widget_->minesCount();
    parameters_.maximum_neighbors    = parameters_widget_->maximumNeighbors();
    parameters_.allow_disjoint_graph = parameters_widget_->disjointGraphAllowed();
}
