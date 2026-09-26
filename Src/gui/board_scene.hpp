#pragma once

#include "cell_item.hpp"

#include <QGraphicsScene>
#include <QTimer>

#include <unordered_map>

class BoardScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit BoardScene(QObject* parent = nullptr);

    void registerCellItem(CellItem* cell_item);
    void updateCellItemForCell(const Cell* cell);
    void clear();
    void startAnimation();
    void stopAnimation();
    void setNotAnimated();
    void setAdvancePeriod(int period);

signals:
    void cellClicked(std::size_t id, QGraphicsSceneMouseEvent* event);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

protected:
    using CellsMap = std::unordered_map<const Cell*, CellItem*>;
    CellsMap cell_items_;
    QTimer   timer_;
    int      advance_period_ = 33;
};
