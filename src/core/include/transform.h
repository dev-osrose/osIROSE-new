#pragma once

#include "enumerate.h"
#include "array_wrapper.h"

#include <utility>

namespace Core {
template <typename T, typename U, size_t N>
constexpr auto transform(const std::array<T, N>& input, U&& pred) {
    std::array<decltype(pred(std::declval<T>())), N> res{};
    for (const auto& [i, it] : enumerate(input)) {
        res[i] = pred(it);
    }
    return res;
}

template <template <typename, size_t> typename V, typename T, size_t N, size_t M, size_t L, typename U>
constexpr auto transform(const array_wrapper<V<T, N>, M, L>& input, U&& pred) {
    V<decltype(pred(std::declval<T>())), L> res{};
    for (const auto& [i, it] : enumerate(input)) {
        res[i] = pred(it);
    }
    return res;
}

template <template <typename, size_t> typename V, typename T, size_t N, size_t M, size_t L, typename U>
constexpr auto transform(const array_wrapper<const V<T, N>, M, L>& input, U&& pred) {
    V<decltype(pred(std::declval<T>())), L> res{};
    for (const auto& [i, it] : enumerate(input)) {
        res[i] = pred(it);
    }
    return res;
}
}
