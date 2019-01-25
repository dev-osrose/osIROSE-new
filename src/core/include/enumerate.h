#pragma once

#include <tuple>

namespace Core {
namespace {
using std::begin;
using std::end;
}
template <typename T,
          typename TIter = decltype(begin(std::declval<T>())),
          typename = decltype(end(std::declval<T>()))>
constexpr auto enumerate(T && iterable)
{
    struct iterator
    {
        size_t i;
        TIter iter;
        bool operator != (const iterator & other) const { return iter != other.iter; }
        void operator ++ () { ++i; ++iter; }
        auto operator * () const { return std::tie(i, *iter); }
    };
    struct iterable_wrapper
    {
        T iterable;
        auto begin() { using Core::begin; return iterator{ 0, begin(iterable) }; }
        auto end() { using Core::end; return iterator{ 0, end(iterable) }; }
    };
    return iterable_wrapper{ std::forward<T>(iterable) };
}
}
