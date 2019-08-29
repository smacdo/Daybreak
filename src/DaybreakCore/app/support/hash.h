#pragma once
#include <functional>

#define DAYBREAK_MAKE_HASHABLE(type, ...) \
    namespace std {\
        template<> struct hash<type> {\
            std::size_t operator()(const type &t) const {\
                return combine_hash(0, __VA_ARGS__);\
            }\
        };\
    }

namespace Daybreak
{
    /** Calculate a hash value by hashing the given object and combining it with the provided seed. */
    template <typename T, typename... Rest>
    std::size_t combine_hash(std::size_t seed, const T& v, Rest... rest)
    {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }

    /** Empty. */
    std::size_t combine_hash(std::size_t seed)
    {
        return seed;
    }
}
