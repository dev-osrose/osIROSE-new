#pragma once

#include <tuple>

namespace Core {

template <class T, class Tuple>
struct Index;

template <class T, class... Types>
struct Index<T, std::tuple<T, Types...>> {
    static constexpr const std::size_t value = 0;
};

template <class T, class U, class... Types>
struct Index<T, std::tuple<U, Types...>> {
    static constexpr const std::size_t value = 1 + Index<T, std::tuple<Types...>>::value;
};

}
