#include "board_view.hpp"

#include <QWheelEvent>

BoardView::BoardView(QWidget *parent)
    : QGraphicsView{parent}
    , positive_factor_{1.1}
    , negative_factor_{1. / positive_factor_}
    , click_location_{}
    , scene_center_{}
    , is_scrolling_{false}
{
    this->setTransformationAnchor (QGraphicsView::AnchorUnderMouse);
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
}

void BoardView::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0){
        scale(positive_factor_, positive_factor_);
    } else {
        scale(negative_factor_, negative_factor_);
    }
}

void BoardView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton) {
        is_scrolling_ = true;
        click_location_ = event->pos();
        scene_center_ = mapToScene(viewport()->rect().center());
    }
    QGraphicsView::mousePressEvent(event);
}

void BoardView::mouseMoveEvent(QMouseEvent *event)
{
    if (is_scrolling_) {
        centerOn(scene_center_ - event->pos() + click_location_);
    }
    QGraphicsView::mouseMoveEvent(event);
}

void BoardView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton){
        is_scrolling_ = false;
    }
    QGraphicsView::mouseReleaseEvent(event);
}
