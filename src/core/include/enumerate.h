#pragma once

#include <tuple>

namespace Core {
namespace {
using std::begin;
using std::end;

template <typename TIter>
struct iterator
{
    size_t i;
    TIter iter;
    bool operator != (const iterator & other) const { return iter != other.iter; }
    void operator ++ () { ++i; ++iter; }
    auto operator * () const { return std::tie(i, *iter); }
};

template <typename T, typename TIter>
struct iterable_wrapper
{
    iterable_wrapper(T&& iterable) : iterable(std::forward<T>(iterable)) {}
    T iterable;
};

template <typename T, typename TIter>
constexpr auto begin(iterable_wrapper<T, TIter>& it) { return iterator<TIter>{ 0, begin(it.iterable) }; }
template <typename T, typename TIter>
constexpr auto end(iterable_wrapper<T, TIter>& it) { return iterator<TIter>{ 0, end(it.iterable) }; }
}

template <typename T,
          typename TIter = decltype(begin(std::declval<T>())),
          typename = decltype(end(std::declval<T>()))>
constexpr auto enumerate(T && iterable)
{
    return iterable_wrapper<T, TIter>{ std::forward<T>(iterable) };
}
}
