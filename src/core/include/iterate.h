#pragma once

#include <tuple>
#include <memory>
#include <type_traits>
#include <utility>

#include "invoke.h"

namespace Core {

namespace detail {
template <class F, class Tuple, std::size_t... I>
constexpr decltype(auto) apply_impl(F&& f, Tuple&& t, std::index_sequence<I...>) {
    return invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))...);
}
}

template <class F, class Tuple>
constexpr decltype(auto) apply(F &&f, Tuple&& t) {
    return detail::apply_impl(std::forward<F>(f), std::forward<Tuple>(t), std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
}

template <class F, class... Ts, std::size_t... Is>
constexpr void for_each_in_tuple(const std::tuple<Ts...> &tuple, F f, std::index_sequence<Is...>) {
    using expander = int[];
    (void) expander{0, ((void)f(Is, std::get<Is>(tuple)), 0)...};
}

template <class F, class... Ts>
constexpr void for_each_in_tuple(const std::tuple<Ts...> &tuple, F f) {
    for_each_in_tuple(tuple, f, std::make_index_sequence<sizeof...(Ts)>());
}

constexpr int my_strcmp(const char *str1, const char *str2) {
    int i = 0;
    for (; str1[i] && str2[i] && str1[i] == str2[i]; ++i) {}
    if (str1[i] == str2[i]) return 0;
    if (str1[i] < str2[i]) return -1;
    return 1;
}

template <class T, class Tuple>
struct IndexOfType;

template <class T, class... Types>
struct IndexOfType<T, std::tuple<T, Types...>> {
    static const std::size_t value = 0;
};

template <class T, class U, class... Types>
struct IndexOfType<T, std::tuple<U, Types...>> {
    static const std::size_t value = 1 + IndexOfType<T, std::tuple<Types...>>::value;
};

}
