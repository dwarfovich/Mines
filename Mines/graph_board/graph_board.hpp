#ifndef GRAPHBOARD_HPP
#define GRAPHBOARD_HPP

#include "standard_board.hpp"
#include "triangulator.hpp"

class GraphBoard : public StandardBoard
{
    Q_OBJECT

public:
    std::unique_ptr<IBoard> clone() const override;
    QObject                *toQObject() override;
    const QString          &id() const override;
    const QString          &name() const override;
    QWidget                *createParametersWidget() const override;
    void                    generate(QWidget *parameters_widget) override;
    void                    drawBoard(BoardScene *scene) override;

protected: // methods
    std::vector<size_t> neighborIds(size_t id) const override;
    virtual void        generatePoints();
    virtual void        drawNodes(BoardScene *scene) const;
    virtual void        drawEdges(BoardScene *scene, const std::vector<QPointF> &node_coordinates) const;
    void                FormNeighbors(const Triangulator &triangulator);

protected: // data
    std::vector<QPointF> points_;
    using NeighborsVector = std::vector<std::vector<size_t>>;
    NeighborsVector neighbors_;
    Triangulator    triangulator_;
    double          grid_step_ = 64.;
    QRectF          bounding_rect_;
};

#endif // GRAPHBOARD_HPP
