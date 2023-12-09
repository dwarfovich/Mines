#pragma once

#include <utility>

template<typename T, typename U, typename THasher = std::hash<T>, typename UHasher = std::hash<U>>
struct StdPairHasher
{
    size_t operator()(const std::pair<T, U>& pair) const
    {
        const auto& first_hash = THasher()(pair.first);
        return first_hash ^ (UHasher()(pair.second) + 0x9e3779b9 + (first_hash << 6) + (first_hash >> 2));
    }
};
