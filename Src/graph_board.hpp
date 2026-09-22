#pragma once

#include "board_state.hpp"
#include "edge.hpp"
#include "edge_item.hpp"
#include "graph_board.hpp"
#include "graph_boards_constants.hpp"
#include "graph_boards_parameters.h"
#include "graph_cell_item.hpp"
#include "graph_parameters_widget.h"
#include "id_based_board.hpp"
#include "qpointf_hasher.hpp"

#include "gui/board_scene.hpp"

#include <random>
#include <unordered_set>

template <typename ParametersWidgetType = GraphParametersWidget>
class GraphBoard : public IdBasedBoard<Cell, ParametersWidgetType> {
public:
    const QString& id() const override;
    const QString& name() const override;
    void           generate() override;
    void           setupScene(BoardScene* scene) override;

protected:  // methods
    std::vector<std::size_t> neighborIds(std::size_t id) const override;
    virtual void             generatePoints();
    virtual void             formNeighbors();
    virtual void             setupCellItems();
    virtual void             setupParameters();

protected:  // data
    std::vector<QPointF> points_;
    using NeighborsVector = std::vector<std::vector<std::size_t>>;
    NeighborsVector              neighbors_;
    QRectF                       bounding_rect_;
    mutable GraphBoardParameters parameters_;
};

template <typename ParametersWidgetType>
const QString& GraphBoard<ParametersWidgetType>::id() const
{
    static const QString id{QStringLiteral("Graph")};
    return id;
}

template <typename ParametersWidgetType>
const QString& GraphBoard<ParametersWidgetType>::name() const
{
    static const QString id{QStringLiteral("Graph")};
    return id;
}

template <typename ParametersWidgetType>
void GraphBoard<ParametersWidgetType>::generate()
{
    auto* parameters_widget = this->parametersWidget();

    if (!parameters_widget) {
        Q_ASSERT(false);
        return;
    }

    setupParameters();

    this->board_state_ = {};
    this->flags_ = 0;
    this->board_state_.mines = parameters_.mines_count;
    this->board_state_.empty_cells = parameters_.nodes_count - this->board_state_.mines;

    this->initializeCells(parameters_.nodes_count);
    this->randomize();

    generatePoints();
    formNeighbors();

    this->board_state_.game_state = GameState::Playing;
}

template <typename ParametersWidgetType>
void GraphBoard<ParametersWidgetType>::setupScene(BoardScene* scene)
{
    Q_ASSERT(scene);

    setupCellItems();

    const auto                                      sprite_size = SpriteCellItem::size();
    std::unordered_map<std::size_t, GraphCellItem*> id_to_item_map;
    for (std::size_t id = 0; id < points_.size(); ++id) {
        auto* node_item = new GraphCellItem{this->cellById(id)};
        node_item->setZValue(constants::graph_board::node_z_value);
        node_item->setPos(points_[id]);
        scene->registerCellItem(node_item);
        id_to_item_map[id] = node_item;
    }

    std::unordered_set<Edge, EdgeHasher> createdEdges;
    for (const auto& [id, item] : id_to_item_map) {
        const auto& neighbors = neighbors_[id];
        const auto& point1 = points_[id];
        for (const auto& buddy_id : neighbors) {
            item->addBuddy(id_to_item_map[buddy_id]);
            const auto& point2 = points_[buddy_id];
            Edge        edge{point1, point2};
            auto        iter = createdEdges.find(edge);
            if (createdEdges.find(edge) == createdEdges.cend()) {
                createdEdges.insert(edge);
                auto* edge_item = new EdgeItem{edge};
                scene->addItem(edge_item);
                item->addBuddy(edge_item);
                id_to_item_map[buddy_id]->addBuddy(edge_item);
            }
        }
    }

    using namespace constants::graph_board;
    scene->setSceneRect(bounding_rect_.adjusted(-bounding_side_adjustment,
                                                -bounding_side_adjustment,
                                                bounding_side_adjustment,
                                                bounding_side_adjustment));
}

template <typename ParametersWidgetType>
std::vector<std::size_t> GraphBoard<ParametersWidgetType>::neighborIds(std::size_t id) const
{
    return neighbors_[id];
}

template <typename ParametersWidgetType>
void GraphBoard<ParametersWidgetType>::generatePoints()
{
    bounding_rect_.setLeft(std::numeric_limits<double>::max());
    bounding_rect_.setRight(std::numeric_limits<double>::lowest());
    bounding_rect_.setTop(std::numeric_limits<double>::max());
    bounding_rect_.setBottom(std::numeric_limits<double>::lowest());

    points_.resize(this->cells_.size());
    std::uniform_real_distribution<> distribution(0, constants::graph_board::random_points_bounding_side);
    for (auto& point : points_) {
        point = {distribution(this->random_generator_), distribution(this->random_generator_)};
        bounding_rect_.setLeft(std::min(point.x(), bounding_rect_.left()));
        bounding_rect_.setRight(std::max(point.x(), bounding_rect_.right()));
        bounding_rect_.setTop(std::min(point.y(), bounding_rect_.top()));
        bounding_rect_.setBottom(std::max(point.y(), bounding_rect_.bottom()));
    }
}

template <typename ParametersWidgetType>
void GraphBoard<ParametersWidgetType>::formNeighbors()
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
        auto&      neighbor_ids = neighbors_[i];
        const auto max_neighbors = max_distribution(this->random_generator_);
        while (neighbor_ids.size() <= max_neighbors) {
            const auto& max_attempts = constants::graph_board::max_attempts_to_find_neighbor;
            auto        neighbor_id = 0;
            std::size_t attempt = 0;
            do {
                neighbor_id = neighbor_distribution(this->random_generator_);
            } while (((neighbor_id == i) || ::contains(neighbor_ids, neighbor_id)) && (++attempt < max_attempts));
            if (attempt < max_attempts) {
                neighbor_ids.push_back(neighbor_id);
                if (!::contains(neighbors_[neighbor_id], i)) {
                    neighbors_[neighbor_id].push_back(i);
                }
            } else {  // Failed to find correct neighbor.
                break;
            }
        }
    }
}

template <typename ParametersWidgetType>
void GraphBoard<ParametersWidgetType>::setupCellItems()
{
    SpriteCellItem::setSprites(constants::graph_board::sprites_path);
    const auto   sprite_size = SpriteCellItem::size();
    QPainterPath path;
    path.addEllipse(-sprite_size / 2., -sprite_size / 2., sprite_size, sprite_size);
    SpriteCellItem::setShape(path);
}

template <typename ParametersWidgetType>
void GraphBoard<ParametersWidgetType>::setupParameters()
{
    auto* parameters_widget = this->parametersWidget();
    if (!parameters_widget) {
        Q_ASSERT(false);
        return;
    }

    parameters_.nodes_count = parameters_widget->nodesCount();
    parameters_.mines_count = parameters_widget->minesCount();
    // TODO:
//    parameters_.maximum_neighbors = parameters_widget->maximumNeighbors();
    // TODO:
   // parameters_.allow_disjoint_graph = parameters_widget->allowDisjointGraph();
}
