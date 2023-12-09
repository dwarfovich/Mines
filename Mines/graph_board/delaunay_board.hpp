#ifndef DELAUNAY_BOARD_HPP
#define DELAUNAY_BOARD_HPP

#include "standard_board.hpp"
#include "triangulator.hpp"

class DelaunayBoard : public StandardBoard
{
    Q_OBJECT

public:
    std::unique_ptr<IBoard> create() const override;
    QObject                *toQObject() override;
    const QString          &id() const override;
    const QString          &name() const override;
    QWidget                *createParametersWidget() const override;
    void                    generate(QWidget *parameters_widget) override;
    void                    drawBoard(BoardScene *scene) override;

protected: // methods
    std::vector<size_t> neighborIds(size_t id) const override;
    void                generatePoints();
    void                FormNeighbors(const Triangulator &triangulator);

protected: // data
    std::vector<QPointF> points_;
    using NeighborsVector = std::vector<std::vector<size_t>>;
    NeighborsVector neighbors_;
    Triangulator    triangulator_;
    double          grid_step_ = 64.;
    QRectF          bounding_rect_;
};

#endif // DELAUNAY_BOARD_HPP
