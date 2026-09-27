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

template <typename CellType = GraphCell, typename ParametersWidgetType = GraphParametersWidget>
class GraphBoard : public IdBasedBoard<CellType, ParametersWidgetType> {
public:
    const QString& id() const override;
    const QString& name() const override;
    void           generate() override;
    void           setupScene(BoardScene* scene) override;

protected:  // methods
    std::vector<std::size_t> neighborIds(std::size_t id) const override;
    virtual void             assignCoordinatesToCells();
    virtual void             formNeighbors();
    virtual void             setupCellItemsSprites();
    virtual void             setupParameters();
    virtual void             updateBoundingRect(double cell_size);
    virtual const QRectF     boundingRect() const
    {
        return bounding_rect_;
    }

protected:  // data
    using AbstractBoard<ParametersWidgetType>::board_state_;
    using IdBasedBoard<CellType, ParametersWidgetType>::flags_;
    using IdBasedBoard<CellType, ParametersWidgetType>::cells_;
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

    board_state_ = {};
    board_state_.mines = parameters_.mines_count;
    board_state_.empty_cells = parameters_.nodes_count - board_state_.mines;
    flags_ = 0;

    this->initializeCells(parameters_.nodes_count);
    this->randomize();

    assignCoordinatesToCells();
    formNeighbors();

    board_state_.game_state = GameState::Playing;
}

template <typename CellType, typename ParametersWidgetType>
void GraphBoard<CellType, ParametersWidgetType>::setupScene(BoardScene* scene)
{
    Q_ASSERT(scene);

    setupCellItemsSprites();

    const auto                                      sprite_size = SpriteCellItem::size();
    std::unordered_map<std::size_t, GraphCellItem*> id_to_item_map;
    for (std::size_t id = 0; id < this->cells_.size(); ++id) {
        auto* node_item = new GraphCellItem{this->cellById(id)};
        node_item->setZValue(constants::graph_board::node_z_value);
        scene->registerCellItem(node_item);
        id_to_item_map[id] = node_item;
    }

    std::unordered_set<Edge, EdgeHasher> createdEdges;
    for (const auto& [id, item] : id_to_item_map) {
        const auto& neighbors = neighbors_[id];
        const auto& point1 = this->cells_[id].coordinates;
        for (const auto& buddy_id : neighbors) {
            item->addBuddy(id_to_item_map[buddy_id]);
            const auto& point2 = this->cells_[buddy_id].coordinates;
            Edge        edge{point1, point2};
            auto        iter = createdEdges.find(edge);
            if (createdEdges.find(edge) == createdEdges.cend()) {
                createdEdges.insert(edge);
                auto* edge_item = new EdgeItem{edge};
                edge_item->setPointItem1(item);
                edge_item->setPointItem2(id_to_item_map[buddy_id]);
                scene->addItem(edge_item);
                item->addBuddy(edge_item);
                id_to_item_map[buddy_id]->addBuddy(edge_item);
            }
        }
    }

    scene->setSceneRect(boundingRect());
}

template <typename CellType, typename ParametersWidgetType>
std::vector<std::size_t> GraphBoard<CellType, ParametersWidgetType>::neighborIds(std::size_t id) const
{
    return neighbors_[id];
}

template <typename CellType, typename ParametersWidgetType>
void GraphBoard<CellType, ParametersWidgetType>::assignCoordinatesToCells()
{
    // Bridson's Poisson disk sampling
    constexpr std::size_t max_attempts_to_find_candidate = 30;
    constexpr double      min_distance = 64.;
    const double          cell_size = min_distance / std::sqrt(2.0);
    const std::size_t     target_points_count = parameters_.nodes_count;
    const double          field_side = target_points_count * cell_size;
    const std::size_t     grid_side = std::ceil(field_side / cell_size);

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
                    const auto& neighbor_point = this->cells_[neighbor_index].coordinates;
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
    cells_.front().coordinates = QPointF{point_distribution(generator), point_distribution(generator)};
    std::size_t assigned_points = 1;
    active_list.push_back(0);
    const auto [cell_x, cell_y] = gridLocation(cells_.front().coordinates);
    grid[cell_y][cell_x] = 0;

    while (!active_list.empty() && assigned_points < target_points_count) {
        SizeTDistribution active_distribution{0, active_list.size() - 1};
        const auto        active_list_index = active_distribution(generator);
        const auto        sample_index = active_list[active_list_index];
        bool              candidate_succeeded = false;
        for (size_t attempt = 0; attempt < max_attempts_to_find_candidate; ++attempt) {
            const auto    angle = angle_distribution(generator);
            const auto    distance = distance_distribution(generator);
            const QPointF direction{std::cos(angle), std::sin(angle)};
            const QPointF candidate = cells_[sample_index].coordinates + direction * distance;
            if (isValidCandidate(candidate)) {
                cells_[assigned_points].coordinates = candidate;
                active_list.push_back(assigned_points);
                auto [candidate_x, candidate_y] = gridLocation(candidate);
                grid[candidate_y][candidate_x] = assigned_points;
                candidate_succeeded = true;
                ++assigned_points;
                break;
            }
        }

        if (!candidate_succeeded) {
            active_list.erase(active_list.begin() + active_list_index);
        }
    }

    updateBoundingRect(min_distance);
}

template <typename CellType, typename ParametersWidgetType>
void GraphBoard<CellType, ParametersWidgetType>::updateBoundingRect(double cell_size)
{
    const auto& [min_x,
                 max_x] = std::minmax_element(cells_.begin(), cells_.end(), [](const auto& cell1, const auto& cell2) {
        return cell1.coordinates.x() < cell2.coordinates.x();
    });
    bounding_rect_.setLeft(min_x->x() - cell_size);
    bounding_rect_.setRight(max_x->x() + cell_size);
    const auto& [min_y,
                 max_y] = std::minmax_element(cells_.begin(), cells_.end(), [](const auto& cell1, const auto& cell2) {
        return cell1.coordinates.y() < cell2.coordinates.y();
    });
    bounding_rect_.setTop(min_y->y() - cell_size);
    bounding_rect_.setBottom(max_y->y() + cell_size);
}

template <typename CellType, typename ParametersWidgetType>
void GraphBoard<CellType, ParametersWidgetType>::formNeighbors()
{
    neighbors_.clear();
    neighbors_.resize(cells_.size());

    if (!parameters_.allow_disjoint_graph) {
        for (std::size_t i = 1; i < cells_.size(); ++i) {
            neighbors_[i].push_back(i - 1);
            neighbors_[i - 1].push_back(i);
        }
    }

    std::uniform_int_distribution<std::size_t> neighbor_distribution(0, cells_.size() - 1);
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
            } while (((neighbor_id == i) || contains(neighbor_ids, neighbor_id)) && (++attempt < max_attempts));

            if (attempt < max_attempts) {
                neighbor_ids.push_back(neighbor_id);
                if (!contains(neighbors_[neighbor_id], i)) {
                    neighbors_[neighbor_id].push_back(i);
                }
            } else {  // Failed to find correct neighbor.
                break;
            }
        }
    }
}

template <typename CellType, typename ParametersWidgetType>
void GraphBoard<CellType, ParametersWidgetType>::setupCellItemsSprites()
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
