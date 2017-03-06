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
#include <fstream>
#include <iostream>
#include <exception>

namespace Core {

Config &Config::getInstance(std::string filename) {
  static Config instance(filename);
  return instance;
}

std::string prettify(const std::string &data) {
  std::string res;
  int dist = 0;
  for (auto &it : data) {
    if (it == '{' || it == '[') {
      res.push_back(it);
      res.push_back('\n');
      ++dist;
      for (int i = 0; i < dist; ++i) res.push_back(' ');
    } else if (it == '}' || it == ']') {
      res.push_back('\n');
      for (int i = 0; i < dist; ++i) res.push_back(' ');
      res.push_back(it);
      --dist;
    } else if (it == ',') {
      res.push_back(it);
      res.push_back('\n');
      for (int i = 0; i < dist; ++i) res.push_back(' ');
    } else if (it == ':') {
      res.push_back(' ');
      res.push_back(it);
      res.push_back(' ');
    } else
      res.push_back(it);
  }
  return res;
}

Config::Config(std::string filename) : Configuration(), file_(filename) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  std::fstream in(file_.c_str(), std::ios::in);
  std::shared_ptr<spdlog::logger> logger_ = CLog::GetLogger(log_type::GENERAL).lock();
  if (!in.is_open()) {
    logger_->debug("file {} not found. Creating one", filename );
	std::fstream out(file_.c_str(), std::ios::out | std::ios::trunc);
    if (!out.is_open()) throw std::runtime_error("file not found");
	std::string json;
	pbjson::pb2json(this, json);
    out << prettify(json);
  } else {
	std::string json, err;
	std::getline(in, json, static_cast<char>(in.eof()));
    if (pbjson::json2pb(json, this, err) < 0) {
      logger_->error("Error while parsing the file: {}", err);
      throw std::runtime_error("Error while parsing the file");
    }
  }
}

Config::~Config() {
  google::protobuf::ShutdownProtobufLibrary();
}

}
