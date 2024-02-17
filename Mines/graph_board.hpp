#pragma once

#include "id_based_board.hpp"
#include "graph_boards_parameters.h"

#include <random>

class GraphParametersWidget;

class GraphBoard : public IdBasedBoard<Cell>
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
    virtual void        generatePoints();
    virtual void        formNeighbors();
    virtual void        setupCellItems();
    virtual void        setupParameters();

protected: // data
    std::vector<QPointF> points_;
    using NeighborsVector = std::vector<std::vector<size_t>>;
    NeighborsVector                neighbors_;
    QRectF                         bounding_rect_;
    mutable GraphParametersWidget *parameters_widget_ = nullptr;
    mutable GraphBoardParameters   parameters_;
};
