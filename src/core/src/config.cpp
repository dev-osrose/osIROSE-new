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

#include "config.h"
#include <exception>
#include <fstream>
#define CONFIGURU_IMPLEMENTATION 1
#include <configuru.hpp>

namespace Core {

Config &Config::getInstance(std::string filename) {
  static Config instance(filename);
  return instance;
}

Config::Config(std::string filename) : file_(filename) {
  std::fstream in(file_.c_str(), std::ios::in);
  std::shared_ptr<spdlog::logger> logger_ = CLog::GetLogger(log_type::GENERAL).lock();
  auto cfg = configuru::serialize(config_);
  if (!in.is_open()) {
    logger_->info("File {} not found. Creating configuration file.", filename);
    configuru::dump_file(filename, cfg, configuru::JSON);
  } else {
    in.close();
    cfg = configuru::parse_file(filename, configuru::JSON);
    configuru::deserialize(&config_, cfg, [logger_](std::string error) {
        logger_->error(error);
    });
    Config::Configuration config;
  }
}

Config::~Config() {
}

}
