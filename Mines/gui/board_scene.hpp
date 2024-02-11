#ifndef BOARDSCENE_HPP
#define BOARDSCENE_HPP

#include "cell_item.hpp"

#include <QGraphicsScene>
#include <QTimer>

#include <unordered_map>

class BoardScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit BoardScene(QObject* parent = nullptr);

    void registerCellItem(CellItem* cell_item);
    void updateCellItemForCell(Cell* cell);
    void clear();
    void startAnimation();
    void stopAnimation();
    void setNotAnimated();
    void setAdvancePeriod(int period);

signals:
    void cellItemClicked(CellItem* cell_item, QGraphicsSceneMouseEvent* event);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

protected:
    std::unordered_map<const Cell*, CellItem*> cell_items_;
    QTimer                                     timer_;
    int                                      advance_period_ = 0;
};

#endif // BOARDSCENE_HPP
