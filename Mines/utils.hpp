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

double euclideanDistance(const QPointF& p1, const QPointF& p2);

template<typename Container, typename Element>
bool contains(const Container& c, const Element& e)
{
    using namespace std;
    return (find(cbegin(c), cend(c), e) != cend(c));
}

#endif // UTILS_HPP
