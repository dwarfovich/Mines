#ifndef DELAUNAY_BOARD_HPP
#define DELAUNAY_BOARD_HPP

#include "graph_board.hpp"
#include "triangulator.hpp"

class DelaunayParametersWidget;

class DelaunayBoard : public GraphBoard
{
    Q_OBJECT

public:
    const QString &id() const override;
    const QString &name() const override;
    QWidget       *parametersWidget() const override;
    void           generate() override;

protected: // methods
    std::vector<std::size_t> neighborIds(std::size_t id) const override;
    void                formNeighbors(const Triangulator &triangulator);
    void                setupParameters() override;


protected: // data
    Triangulator                      triangulator_;
    mutable DelaunayParametersWidget *parameters_widget_ = nullptr;
};

#endif // DELAUNAY_BOARD_HPP
