#ifndef GRAPHBOARD_HPP
#define GRAPHBOARD_HPP

#include "standard_board.hpp"
#include "triangulator.hpp"

class GraphBoard : public StandardBoard
{
    Q_OBJECT

public:
    std::unique_ptr<IBoard> clone() const override;
    QObject *toQObject() override;
    const QString &id() const override;
    const QString &name() const override;
    QWidget *createParametersWidget() const override;
    void generate(QWidget *parameters_widget) override;
    void drawBoard(BoardScene *scene) override;

protected:
    std::vector<size_t> neighborIds(size_t id) const override;
    virtual void generatePoints();
    virtual void drawNodes(BoardScene* scene) const;
    virtual void drawEdges(BoardScene* scene, const std::vector<QPointF>& node_coordinates) const;

protected:
    std::vector<QPointF> points_;
    EdgeVector neighbors_;
    Triangulator triangulator_;
    double grid_step_ = 64.;
};

#endif // GRAPHBOARD_HPP
