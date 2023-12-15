#include "graph_board.h"
#include "graph_parameters_widget.h"
#include "graph_boards_constants.h"
#include "graph_cell_item.hpp"
#include "edge.hpp"
#include "edge_item.hpp"
#include "qpointf_hasher.hpp"
#include "gui/board_scene.hpp"

#include <unordered_set>

GraphBoard::GraphBoard() : random_generator_ { random_device_() }
{
}

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
        parameters_widget_ = new GraphParametersWidget {};
    }

    return parameters_widget_;
}

void GraphBoard::generate()
{
    if (!parameters_widget_) {
        Q_ASSERT(false);
        return;
    }

    board_state_             = {};
    flags_                   = 0;
    board_state_.mines       = parameters_widget_->minesCount();
    size_t cells_counter     = parameters_widget_->nodesCount();
    board_state_.empty_cells = cells_counter - board_state_.mines;
    grid_step_               = constants::graph_board::grid_step;

    initialize(cells_counter);
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
    std::unordered_map<size_t, GraphCellItem *> id_to_item_map;
    const int                                   node_z_value = 2;
    for (size_t id = 0; id < points_.size(); ++id) {
        auto *node_item = new GraphCellItem { cellById(id) };
        node_item->setZValue(node_z_value);
        const auto &location = points_[id];
        node_item->setPos(location.x() - sprite_size / 2., location.y() - sprite_size / 2.);
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
}

std::vector<size_t> GraphBoard::neighborIds(size_t id) const
{
    return neighbors_[id];
}

void GraphBoard::generatePoints()
{
    double side       = std::sqrt(double(cells_.size()));
    double bound_size = grid_step_ * std::sqrt(side);

    std::uniform_real_distribution<> distribution(0, bound_size);

    bounding_rect_.setLeft(std::numeric_limits<double>::max());
    bounding_rect_.setRight(std::numeric_limits<double>::lowest());
    bounding_rect_.setTop(std::numeric_limits<double>::max());
    bounding_rect_.setBottom(std::numeric_limits<double>::lowest());
    points_.resize(cells_.size());
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
    Q_ASSERT(parameters_widget_);

    const auto &max_neighbors = parameters_widget_->maximumNeighbors();
    neighbors_.clear();
    neighbors_.resize(points_.size());

    if (!parameters_widget_->disjointGraphAllowed()) {
        size_t current_neighbors = 0;
        for (size_t i = 1; i < points_.size(); ++i) {
            neighbors_[i].push_back(i - 1);
            neighbors_[i - 1].push_back(i);
            ++current_neighbors;
        }
    }

    std::uniform_int_distribution<size_t> neighbor_distribution(0, points_.size() - 1);
    std::uniform_int_distribution<size_t> distribution(parameters_widget_->disjointGraphAllowed() ? 0 : 1, 4);
    for (size_t i = 0; i < neighbors_.size(); ++i) {
        auto &neighbor_ids  = neighbors_[i];
        auto  max_neighbors = distribution(random_generator_);
        while (neighbor_ids.size() <= max_neighbors) {
            size_t neighbor_id  = 0;
            size_t max_attempts = 50;
            size_t attempt      = 0;
            do {
                neighbor_id = neighbor_distribution(random_generator_);
            } while (((neighbor_id == i) || ::contains(neighbor_ids, neighbor_id)) && (++attempt < max_attempts));
            if (attempt < max_attempts) {
                neighbor_ids.push_back(neighbor_id);
                if (!::contains(neighbors_[neighbor_id], i)) {
                    neighbors_[neighbor_id].push_back(i);
                }
            }
        }
    }
}

void GraphBoard::setupCellItems()
{
    SpriteCellItem::setSprites(constants::graph_board::sprites_path);
    int          sprite_size = SpriteCellItem::size();
    QPainterPath path;
    path.addEllipse(0., 0., sprite_size, sprite_size);
    SpriteCellItem::setShape(path);
}
