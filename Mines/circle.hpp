#pragma once

#include <QPointF>

struct Circle
{
    bool contains(const QPointF& point) const;

    QPointF center;
    double  radius;
};