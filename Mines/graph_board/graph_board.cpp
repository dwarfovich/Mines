#include "graph_board.hpp"
#include "cell.hpp"
#include "gui/board_scene.hpp"
#include "gui/standard_cell_item.hpp"
#include "graph_board_parameters_widget.hpp"

#include <QGraphicsLineItem>

#include <random>
#include <unordered_map>

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
    static const QString id {"Graph"};
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
    auto widget = qobject_cast<GraphBoardParametersWidget*>(parameters_widget);
    if (widget) {
        board_state_ = {};
        flags_ = 0;
        board_state_.mines = widget->minesCount();
        size_t cells_counter = widget->nodesCount();
        board_state_.empty_cells = cells_counter - board_state_.mines;
        grid_step_ = widget->gridStep();

        initialize(cells_counter);
        randomize();

        generatePoints();
        triangulator_.triangulate(points_);
        neighbors_ = triangulator_.getEdges();
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
    points_.resize(cells_.size());
    double side = std::sqrt(double(cells_.size()));
    double bound_size = grid_step_ * std::sqrt(side);
    std::random_device device;
    std::mt19937 generator {device()};
    std::uniform_real_distribution<> distribution (0, bound_size);
    for (auto& point : points_) {
        point = {distribution(generator), distribution(generator)};
    }
}

void GraphBoard::drawNodes(BoardScene *scene) const
{
    StandardCellItem::setSprites(":/gfx/cells_round.png");
    int sprite_size = StandardCellItem::size();
    QPainterPath path;
    path.addEllipse(0., 0., sprite_size, sprite_size);
    StandardCellItem::setShape(path);

    const int node_z_value = 2;
    for (size_t id = 0; id < cells_.size(); ++id) {
            auto* node_item = new StandardCellItem {cellById(id)};
            node_item->setZValue(node_z_value);
            const auto& location = points_[id];
            node_item->setPos(location.x() - sprite_size/2., location.y() - sprite_size/2.);
            scene->registerCellItem(node_item);
    }
}

void GraphBoard::drawEdges(BoardScene *scene, const std::vector<QPointF> &node_coordinates) const
{
    for (size_t id = 0; id < cells_.size(); ++id) {
        for (const auto& second_id : neighbors_[id]) {
            auto point1 = node_coordinates[id];
            auto point2 = node_coordinates[second_id];
            auto* item = new QGraphicsLineItem {point1.x(), point1.y(), point2.x(), point2.y()};
            scene->addItem(item);
        }
    }
}

std::vector<size_t> GraphBoard::neighborIds(size_t id) const
{
    return neighbors_[id];
}
