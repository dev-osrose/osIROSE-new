#pragma once

#include <vector>
#include <type_traits>

namespace Core {

namespace {

template <typename T>
std::true_type create(T);

template <typename T, typename U>
concept bool NarrowConvertible = requires() {
    create<U>({std::declval<T>()});
};

}

template <typename T, typename... Args>
requires(NarrowConvertible<T, Args> &&...)
auto make_vector(T &&value, Args&&... args) -> std::vector<std::remove_reference_t<decltype(value)>> {
    std::vector<std::remove_reference_t<decltype(value)>> vec;
    vec.reserve(sizeof...(args) + 1);
    vec.push_back(std::forward<T>(value));
    (vec.push_back(std::forward<Args>(args)), ...);
    return vec;
}

}
