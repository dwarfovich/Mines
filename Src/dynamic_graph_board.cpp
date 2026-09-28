#include "dynamic_graph_board.hpp"
#include "dynamic_graph_parameters_widget.hpp"

#include "gui/board_scene.hpp"

#include <numbers>

DynamicGraphBoard::DynamicGraphBoard()
{
    connect(&timer_, &QTimer::timeout, this, &DynamicGraphBoard::advanceCells);
}

const QString& DynamicGraphBoard::id() const
{
    static const QString id = QStringLiteral("DynamicGraph");

    return id;
}

const QString& DynamicGraphBoard::name() const
{
    static const QString name = QStringLiteral("Dynamic graph");

    return name;
}

void DynamicGraphBoard::generate()
{
    GraphBoard<DynamicGraphCell, DynamicGraphParametersWidget>::generate();
    std::uniform_real_distribution<qreal> angle_distribution{0., std::numbers::pi * 2.};
    for (auto& cell : cells_) {
        cell.angle = angle_distribution(this->random_generator_);
        cell.speed = parameters_.speed;
    }
}

void DynamicGraphBoard::startGame()
{
    timer_.start(advance_period);
}

void DynamicGraphBoard::stopGame()
{
    timer_.stop();
}

void DynamicGraphBoard::setupParameters()
{
    auto* parameters_widget = parametersWidget();

    parameters_.nodes_count = parameters_widget->nodesCount();
    parameters_.mines_count = parameters_widget->minesCount();
    parameters_.maximum_neighbors = parameters_widget->maximumNeighbors();
    parameters_.allow_disjoint_graph = parameters_widget->allowDisjointGraph();
    parameters_.speed = static_cast<qreal>(parameters_widget->speed()) * user_speed_conversion_coefficient;
}

void DynamicGraphBoard::advanceCells()
{
    for (auto& cell : cells_) {
        const auto&  field = boundingRect();
        const qreal  field_radius = std::hypot(field.width(), field.height()) / 2.0;
        const auto   center = field.center();
        const QLineF lineToCenter = {QPointF{cell.x(), cell.y()}, center};
        if (lineToCenter.length() >= field_radius * critical_radius_coefficient) {
            cell.angle = std::atan2(-lineToCenter.dy(), lineToCenter.dx());
        } else {
            cell.angle += QRandomGenerator::global()->bounded(random_angle_range) - random_angle_range / 2.;
        }
        cell.coordinates.setX(cell.x() + cell.speed * std::cos(cell.angle) * advance_period);
        cell.coordinates.setY(cell.y() - cell.speed * std::sin(cell.angle) * advance_period);
    }
}
