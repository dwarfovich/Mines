#include "graph_board.hpp"
#include "qpointf_hasher.hpp"
#include "cell.hpp"
#include "graph_cell_item.hpp"
#include "graph_board_parameters_widget.hpp"
#include "edge_item.hpp"
#include "gui/board_scene.hpp"

#include <QGraphicsLineItem>

#include <random>
#include <unordered_map>
#include <unordered_set>

std::unique_ptr<IBoard> GraphBoard::clone() const
{
    return std::make_unique<GraphBoard>();
}

QObject *GraphBoard::toQObject()
{
    return this;
}

const QString &GraphBoard::id() const
{
    static const QString id { "Graph" };
    return id;
}

const QString &GraphBoard::name() const
{
    static const QString name = tr("Graph");
    return name;
}

QWidget *GraphBoard::createParametersWidget() const
{
    return new GraphBoardParametersWidget {};
}

void GraphBoard::generate(QWidget *parameters_widget)
{
    auto widget = qobject_cast<GraphBoardParametersWidget *>(parameters_widget);
    if (widget) {
        board_state_             = {};
        flags_                   = 0;
        board_state_.mines       = widget->minesCount();
        size_t cells_counter     = widget->nodesCount();
        board_state_.empty_cells = cells_counter - board_state_.mines;
        grid_step_               = widget->gridStep();

        initialize(cells_counter);
        randomize();

        generatePoints();
        triangulator_.triangulate(points_, bounding_rect_);
        FormNeighbors(triangulator_);
        triangulator_.clear();
        board_state_.game_state = GameState::Playing;
    } else {
        Q_ASSERT(false);
    }
}

void GraphBoard::drawBoard(BoardScene *scene)
{
    drawNodes(scene);
    drawEdges(scene, points_);
}

void GraphBoard::generatePoints()
{
    double             side       = std::sqrt(double(cells_.size()));
    double             bound_size = grid_step_ * std::sqrt(side);
    std::random_device device;
    // std::mt19937                     generator { device() };
    std::mt19937                     generator { 2 };
    std::uniform_real_distribution<> distribution(0, bound_size);

    bounding_rect_.setLeft(std::numeric_limits<double>::max());
    bounding_rect_.setRight(std::numeric_limits<double>::lowest());
    bounding_rect_.setTop(std::numeric_limits<double>::max());
    bounding_rect_.setBottom(std::numeric_limits<double>::lowest());
    points_.resize(cells_.size());
    for (auto &point : points_) {
        point = { distribution(generator), distribution(generator) };
        bounding_rect_.setLeft(std::min(point.x(), bounding_rect_.left()));
        bounding_rect_.setRight(std::max(point.x(), bounding_rect_.right()));
        bounding_rect_.setTop(std::min(point.y(), bounding_rect_.top()));
        bounding_rect_.setBottom(std::max(point.y(), bounding_rect_.bottom()));
    }
}

void GraphBoard::drawNodes(BoardScene *scene) const
{
    SpriteCellItem::setSprites(":/gfx/cells_round.png");
    int          sprite_size = SpriteCellItem::size();
    QPainterPath path;
    path.addEllipse(0., 0., sprite_size, sprite_size);
    SpriteCellItem::setShape(path);

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

void GraphBoard::drawEdges(BoardScene *scene, const std::vector<QPointF> &node_coordinates) const
{
    /* for (size_t id = 0; id < cells_.size(); ++id) {
         for (const auto &second_id : neighbors_[id]) {
             auto  point1 = node_coordinates[id];
             auto  point2 = node_coordinates[second_id];
             auto *item   = new QGraphicsLineItem { point1.x(), point1.y(), point2.x(), point2.y() };
             scene->addItem(item);
         }
     }*/
}

void GraphBoard::FormNeighbors(const Triangulator &triangulator)
{
    std::unordered_map<QPointF, size_t, QPointFHasher> map;
    for (size_t i = 0; i < points_.size(); ++i) {
        map[points_[i]] = i;
    }
    std::vector<std::unordered_set<size_t>> temp_edges_(points_.size());
    for (const auto &triangle : triangulator.triangulation()) {
        const auto &vertices = triangle.vertices();
        size_t      point1   = map[vertices[0]];
        size_t      point2   = map[vertices[1]];
        size_t      point3   = map[vertices[2]];
        temp_edges_[point1].insert(point2);
        temp_edges_[point1].insert(point3);
        temp_edges_[point2].insert(point1);
        temp_edges_[point2].insert(point3);
        temp_edges_[point3].insert(point1);
        temp_edges_[point3].insert(point2);
    }
    neighbors_.resize(points_.size());
    for (size_t i = 0; i < temp_edges_.size(); ++i) {
        neighbors_[i].insert(neighbors_[i].cend(), temp_edges_[i].cbegin(), temp_edges_[i].cend());
    }
}

std::vector<size_t> GraphBoard::neighborIds(size_t id) const
{
    return neighbors_[id];
}
