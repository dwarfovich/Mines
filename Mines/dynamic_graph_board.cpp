#include "dynamic_graph_board.hpp"
#include "dynamic_graph_cell_item.hpp"
#include "edge_item.hpp"
#include "edge.hpp"
#include "buddy_notificator.hpp"
#include "graph_boards_constants.hpp"
#include "dynamic_graph_parameters_widget.h"
#include "gui/board_scene.hpp"

#include <QRandomGenerator>

#include <unordered_set>
#include <numbers>

const QString &DynamicGraphBoard::id() const
{
    static const QString id = QStringLiteral("DynamicGraph");

    return id;
}

const QString &DynamicGraphBoard::name() const
{
    static const QString name = QStringLiteral("Dynamic graph");

    return name;
}

void DynamicGraphBoard::generate()
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

void DynamicGraphBoard::setupScene(BoardScene *scene)
{
    Q_ASSERT(scene);

    setupCellItems();

    const auto                                  sprite_size = SpriteCellItem::size();
    std::unordered_map<size_t, GraphCellItem *> id_to_item_map;
    const int                                   node_z_value = 2;
    for (size_t id = 0; id < points_.size(); ++id) {
        auto *node_item = new DynamicGraphCellItem { cellById(id) };
        node_item->setAngle(QRandomGenerator::global()->bounded(std::numbers::pi * 2));
        node_item->setSpeed(parameters_.speed);
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
                edge_item->setPointItem1(item);
                edge_item->setPointItem2(id_to_item_map[buddy_id]);
                scene->addItem(edge_item);
                item->addBuddy(edge_item);
                id_to_item_map[buddy_id]->addBuddy(edge_item);
            }
        }
    }

    using namespace constants::graph_board;
    scene->setSceneRect(bounding_rect_.adjusted(
        -bounding_side_adjustment, -bounding_side_adjustment, bounding_side_adjustment, bounding_side_adjustment));

    scene->setAdvancePeriod(1000 / 33);
}

QWidget *DynamicGraphBoard::parametersWidget() const
{
    if (!parameters_widget_) {
        parameters_widget_ = new DynamicGraphParametersWidget { &dummy_parent_widget_ };
    }

    return parameters_widget_;
}

void DynamicGraphBoard::setupParameters()
{
    parameters_.nodes_count          = parameters_widget_->nodesCount();
    parameters_.mines_count          = parameters_widget_->minesCount();
    parameters_.maximum_neighbors    = parameters_widget_->maximumNeighbors();
    parameters_.allow_disjoint_graph = parameters_widget_->allowDisjointGraph();
    parameters_.speed =
        static_cast<double>(parameters_widget_->speed()) * constants::graph_board::user_speed_conversion_coefficient;
}
