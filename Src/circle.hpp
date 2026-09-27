#pragma once

#include <QPointF>

class Circle {
public:
    bool contains(const QPointF& point) const;

    QPointF center;
    double  radius;
};