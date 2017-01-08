#pragma once

#include <vector>
#include <type_traits>
#include <utility>

namespace Core {

namespace {

template <bool...>
struct bool_sequence {};

template <bool... Bs>
using bool_and = std::is_same<bool_sequence<Bs...>, bool_sequence<(Bs || true)...>>;

template <typename T>
std::true_type create(T);

template <typename T, typename U>
decltype(create<U>({std::declval<T>()})) test_nonnarrow_conv(int);

template <typename T, typename U>
std::false_type test_nonnarrow_conv(long);

template <typename T, typename U>
using is_nonnarrow_convertible = decltype(test_nonnarrow_conv<T, U>(0));

template <typename T, typename... Ts>
using nonnarrow_convertible = bool_and<is_nonnarrow_convertible<Ts, T>::value...>;

}

template <typename T, typename... Args, std::enable_if_t<nonnarrow_convertible<std::remove_reference_t<T>, Args...>::value, bool>>
std::vector<std::remove_reference_t<T>> make_vector(T &&value, Args&&... args) {
    std::vector<std::remove_reference_t<T>> vec;
    vec.reserve(sizeof...(args) + 1);
    vec.push_back(std::forward<T>(value));
    (vec.push_back(std::forward<Args>(args)), ...);
    return vec;
}

}
