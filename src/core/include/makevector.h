// Copyright 2016 Chirstopher Torres (Raven), L3nn0x
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http ://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * \file makevector
 *
 * \author L3nn0x
 * \date october 2016
 *
 * simple function to create a vector at compile time.
 */

#pragma once

#include <vector>
#include <type_traits>
#include <utility>
#include <initializer_list>

namespace Core {

/*!
 * \brief This function is used to create a vector at compile time.
 * \param[in] all of the elements that should be in the vector
 * \param[out] the vector<common_element> constructed from the input elements
 *
 */

template <typename... Args>
auto make_vector(Args&&... args) {
    std::vector<std::remove_reference_t<std::common_type_t<Args...>>> vec;
    vec.reserve(sizeof...(args));
    (void)std::initializer_list<int>{ (vec.push_back(std::forward<Args>(args)), 0)... };
    return vec;
}

}
