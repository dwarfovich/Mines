#pragma once

#include "board_state.hpp"
#include "edge.hpp"
#include "edge_item.hpp"
#include "graph_board.hpp"
#include "graph_boards_constants.hpp"
#include "graph_boards_parameters.h"
#include "graph_cell.hpp"
#include "graph_cell_item.hpp"
#include "graph_parameters_widget.hpp"
#include "id_based_board.hpp"

#include "gui/board_scene.hpp"

#include <numbers>

template <typename CellType = Cell, typename ParametersWidgetType = GraphParametersWidget>
class GraphBoard : public IdBasedBoard<CellType, ParametersWidgetType> {
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
    virtual void             updateBoundingRect(double cell_size);
    virtual const QRectF     boundingRect() const
    {
        return bounding_rect_;
    }

protected:  // data
    std::vector<QPointF> points_;
    using NeighborsVector = std::vector<std::vector<std::size_t>>;
    NeighborsVector              neighbors_;
    QRectF                       bounding_rect_;
    mutable GraphBoardParameters parameters_;
};

template <typename CellType, typename ParametersWidgetType>
const QString& GraphBoard<CellType, ParametersWidgetType>::id() const
{
    static const QString id{QStringLiteral("Graph")};
    return id;
}

template <typename CellType, typename ParametersWidgetType>
const QString& GraphBoard<CellType, ParametersWidgetType>::name() const
{
    static const QString id{QStringLiteral("Graph")};
    return id;
}

template <typename CellType, typename ParametersWidgetType>
void GraphBoard<CellType, ParametersWidgetType>::generate()
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

template <typename CellType, typename ParametersWidgetType>
void GraphBoard<CellType, ParametersWidgetType>::setupScene(BoardScene* scene)
{
    Q_ASSERT(scene);

    setupCellItems();

    const auto sprite_size = SpriteCellItem::size();

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

    scene->setSceneRect(bounding_rect_);
}

template <typename CellType, typename ParametersWidgetType>
std::vector<std::size_t> GraphBoard<CellType, ParametersWidgetType>::neighborIds(std::size_t id) const
{
    return neighbors_[id];
}

template <typename CellType, typename ParametersWidgetType>
void GraphBoard<CellType, ParametersWidgetType>::generatePoints()
{
    // Bridson's Poisson disk sampling
    constexpr std::size_t max_attempts_to_find_candidate = 30;
    constexpr double      min_distance = 64.;
    const double          cell_size = min_distance / std::sqrt(2.0);
    const std::size_t     target_points_count = parameters_.nodes_count;
    const double          field_side = target_points_count * cell_size;
    const std::size_t     grid_side = std::ceil(field_side / cell_size);

    points_.clear();
    points_.reserve(target_points_count);
    std::vector<std::size_t> active_list;
    active_list.reserve(target_points_count);
    std::vector<std::vector<int>> grid(grid_side, std::vector<int>(grid_side, -1));

    auto gridLocation = [cell_size](const auto& point) -> std::pair<std::size_t, std::size_t> {
        return {static_cast<std::size_t>(point.x() / cell_size), static_cast<std::size_t>(point.y() / cell_size)};
    };

    auto isValidCandidate = [&](const QPointF& candidate) -> bool {
        if (candidate.x() < 0. || candidate.x() >= field_side || candidate.y() < 0.0 || candidate.y() >= field_side) {
            return false;
        }

        const auto& [grid_x, grid_y] = gridLocation(candidate);
        const auto min_grid_x = grid_x >= 2 ? grid_x - 2 : 0;
        const auto max_grid_x = std::min(grid_x + 3, grid_side);
        const auto min_grid_y = grid_y >= 2 ? grid_y - 2 : 0;
        const auto max_grid_y = std::min(grid_y + 3, grid_side);

        for (std::size_t i = min_grid_y; i < max_grid_y; ++i) {
            for (std::size_t j = min_grid_x; j < max_grid_x; ++j) {
                const auto& neighbor_index = grid[i][j];
                if (neighbor_index != -1) {
                    const auto& neighbor_point = points_[neighbor_index];
                    const auto& distance = QLineF{candidate, neighbor_point}.length();
                    if (distance < min_distance) {
                        return false;
                    }
                }
            }
        }

        return true;
    };

    using RealDistribution = std::uniform_real_distribution<double>;
    using SizeTDistribution = std::uniform_int_distribution<std::size_t>;

    RealDistribution point_distribution{0., std::nextafter(field_side, 0.)};
    RealDistribution angle_distribution{0., 2. * std::numbers::pi};
    RealDistribution distance_distribution{min_distance, 2. * min_distance};

    auto& generator = this->random_generator_;
    points_.emplace_back(point_distribution(generator), point_distribution(generator));
    active_list.push_back(0);
    const auto [cell_x, cell_y] = gridLocation(points_.back());
    grid[cell_y][cell_x] = 0;

    while (!active_list.empty() && points_.size() < target_points_count) {
        SizeTDistribution active_distribution{0, active_list.size() - 1};
        const auto        active_list_index = active_distribution(generator);
        const auto        sample_index = active_list[active_list_index];
        bool              candidate_succeeded = false;
        for (size_t attempt = 0; attempt < max_attempts_to_find_candidate; ++attempt) {
            const auto    angle = angle_distribution(generator);
            const auto    distance = distance_distribution(generator);
            const QPointF direction{std::cos(angle), std::sin(angle)};
            const QPointF candidate = points_[sample_index] + direction * distance;
            if (isValidCandidate(candidate)) {
                points_.push_back(candidate);
                active_list.push_back(points_.size() - 1);
                auto [candidate_x, candidate_y] = gridLocation(candidate);
                grid[candidate_y][candidate_x] = points_.size() - 1;
                candidate_succeeded = true;

                break;
            }
        }

        if (!candidate_succeeded) {
            active_list.erase(active_list.begin() + active_list_index);
        }
    }

    updateBoundingRect(min_distance);

    for (int i = 0; i < this->points_.size(); ++i) {
        this->cells_[i]->coordinates.setX(points_[i].x());
        this->cells_[i]->coordinates.setY(points_[i].y());
    }
}

template <typename CellType, typename ParametersWidgetType>
void GraphBoard<CellType, ParametersWidgetType>::updateBoundingRect(double cell_size)
{
    const auto& [min_x, max_x] = std::minmax_element(points_.begin(),
                                                     points_.end(),
                                                     [](const auto& p1, const auto& p2) { return p1.x() < p2.x(); });
    bounding_rect_.setLeft(min_x->x() - cell_size);
    bounding_rect_.setRight(max_x->x() + cell_size);
    const auto& [min_y, max_y] = std::minmax_element(points_.begin(),
                                                     points_.end(),
                                                     [](const auto& p1, const auto& p2) { return p1.y() < p2.y(); });
    bounding_rect_.setTop(min_y->y() - cell_size);
    bounding_rect_.setBottom(max_y->y() + cell_size);
}

template <typename CellType, typename ParametersWidgetType>
void GraphBoard<CellType, ParametersWidgetType>::formNeighbors()
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

template <typename CellType, typename ParametersWidgetType>
void GraphBoard<CellType, ParametersWidgetType>::setupCellItems()
{
    SpriteCellItem::setSprites(constants::graph_board::sprites_path);
    const auto   sprite_size = SpriteCellItem::size();
    QPainterPath path;
    path.addEllipse(-sprite_size / 2., -sprite_size / 2., sprite_size, sprite_size);
    SpriteCellItem::setShape(path);
}

template <typename CellType, typename ParametersWidgetType>
void GraphBoard<CellType, ParametersWidgetType>::setupParameters()
{
    auto* parameters_widget = this->parametersWidget();
    if (!parameters_widget) {
        Q_ASSERT(false);
        return;
    }

    parameters_.nodes_count = parameters_widget->nodesCount();
    parameters_.mines_count = parameters_widget->minesCount();
}
