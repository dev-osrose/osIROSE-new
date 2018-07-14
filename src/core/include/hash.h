#pragma once

#include <functional>
#include <tuple>
#include <utility>

namespace Core {

template <typename T>
constexpr void hash_combine(std::size_t& seed, const T& val) {
    seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <typename T, typename... Args>
constexpr void hash_combine(std::size_t& seed, const T& val, const Args&... vals) {
    hash_combine(seed, val);
    hash_combine(seed, vals...);
}

template <typename... Types>
constexpr std::size_t hash_val(const Types&... args) {
    std::size_t seed = 0;
    hash_combine(seed, args...);
    return seed;
}

template <typename Tuple, std::size_t... I>
constexpr size_t apply_impl(const Tuple& tuple, std::index_sequence<I...>) {
    return hash_val(std::get<I>(tuple)...);
}

struct tuple_hash {
    template <typename... Types>
    std::size_t operator()(const std::tuple<Types...>& k) const {
        return apply_impl(k, std::make_index_sequence<sizeof...(Types)>());
    }
};

}
