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
 * \file logconsole.h
 *
 * \author RavenX8
 * \date march 2016
 *
 * Console logging class
 */
#ifndef __CONSOLE_H_
#define __CONSOLE_H_

#include <ios>
#include <iomanip>
#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Core {

enum class log_type {
  GENERAL,
  NETWORK,
  DATABASE,
  SYSTEM,
  ASSERT,
  SCRIPTLOADER,
};

class CLog {
public:
  static std::weak_ptr<spdlog::logger> GetLogger(log_type _type);
  static void SetLevel(spdlog::level::level_enum _level = spdlog::level::info);

private:
  static spdlog::level::level_enum level_;
};
}

#endif
