#ifndef UTILS_HPP
#define UTILS_HPP

#include <QString>
#include <QPointF>

#include <cmath>

struct QStringHasher
{
    std::size_t operator()(const QString& str) const;
};

inline constexpr auto sqr(auto&& x) noexcept(noexcept(x* x)) -> decltype(x * x)
{
    return x * x;
}

template <typename Type>
double euclideanDistance(const Type& p1, const Type& p2)
{
    return std::sqrt(sqr(p1.x() - p2.x()) + sqr(p1.y() - p2.y()));
}

template<typename Container, typename Element>
bool contains(const Container& c, const Element& e)
{
    using namespace std;
    return (find(cbegin(c), cend(c), e) != cend(c));
}

qreal normalizeAngle(qreal angle);

#endif // UTILS_HPP
