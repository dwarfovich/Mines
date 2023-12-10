#ifndef DELAUNAY_BOARD_HPP
#define DELAUNAY_BOARD_HPP

#include "id_based_board.hpp"
#include "triangulator.hpp"

class DelaunayParametersWidget;

class DelaunayBoard : public IdBasedBoard
{
    Q_OBJECT

public:
    const QString &id() const override;
    const QString &name() const override;
    QWidget       *parametersWidget() const override;
    void           generate() override;
    void           setupScene(BoardScene *scene) override;

protected: // methods
    std::vector<size_t> neighborIds(size_t id) const override;
    void                generatePoints();
    void                formNeighbors(const Triangulator &triangulator);
    void                setupCellItems();

protected: // data
    std::vector<QPointF> points_;
    using NeighborsVector = std::vector<std::vector<size_t>>;
    NeighborsVector                   neighbors_;
    Triangulator                      triangulator_;
    double                            grid_step_ = 64.;
    QRectF                            bounding_rect_;
    mutable DelaunayParametersWidget *parameters_widget_ = nullptr;
};

#endif // DELAUNAY_BOARD_HPP
