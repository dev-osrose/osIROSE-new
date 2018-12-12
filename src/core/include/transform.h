#pragma once

#include "enumerate.h"

namespace Core {
template <typename T, typename U, size_t N>
auto transform(const std::array<T, N>& input, U&& pred) -> std::array<decltype(pred(T{})), N> {
    std::array<decltype(pred(T{})), N> res;
    for (const auto& [i, it] : enumerate(input)) {
        res[i] = pred(it);
    }
    return res;
}
}
