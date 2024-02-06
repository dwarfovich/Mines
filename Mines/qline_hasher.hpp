#pragma once

class QLine;

struct QLineHasher
{
    size_t operator()(const QLine& line) const;
};
