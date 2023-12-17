#pragma once

#include "id_based_board.hpp"

#include <random>

class GraphParametersWidget;

class GraphBoard : public IdBasedBoard
{
    Q_OBJECT

public:
    GraphBoard();

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

protected: // data
    std::vector<QPointF> points_;
    using NeighborsVector = std::vector<std::vector<size_t>>;
    NeighborsVector                neighbors_;
    QRectF                         bounding_rect_;
    std::random_device             random_device_;
    std::mt19937                   random_generator_;
    mutable GraphParametersWidget *parameters_widget_ = nullptr;
};
