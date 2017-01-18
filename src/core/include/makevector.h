#pragma once

#include <vector>
#include <type_traits>
#include <utility>
#include <initializer_list>

namespace Core {

template <typename... Args>
auto make_vector(Args&&... args) {
    std::vector<std::remove_reference_t<std::common_type_t<Args...>>> vec;
    vec.reserve(sizeof...(args));
    (void)std::initializer_list<int>{ (vec.push_back(std::forward<Args>(args)), 0)... };
    return vec;
}

}
