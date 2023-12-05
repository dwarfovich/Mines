#ifndef UTILS_HPP
#define UTILS_HPP

#include <QString>
#include <QPointF>

struct QStringHasher
{
    size_t operator()(const QString& str) const;
};

inline constexpr auto sqr(auto&& x) noexcept(noexcept(x* x)) -> decltype(x * x)
{
    return x * x;
}

inline double euclideanDistance(const QPointF& p1, const QPointF& p2)
{
    return std::sqrt(sqr(p1.x() - p2.x()) + sqr(p1.y() - p2.y()));
}

template<typename Container, typename Element>
bool contains(const Container& c, const Element& e)
{
    using namespace std;
    return (find(cbegin(c), cend(c), e) != cend(c));
}

#endif // UTILS_HPP
