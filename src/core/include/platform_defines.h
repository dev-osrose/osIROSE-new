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

#ifndef _PLATFORM_DEFINES_H_
#define _PLATFORM_DEFINES_H_
#include <chrono>

namespace Core {
  namespace Time {
    inline auto GetTickCount()
    {
      return std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now());
    }
  } // end namespace Time
} // end namespace Core
#endif
