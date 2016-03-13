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

Config::Config(std::string filename) : Configuration(), file_(filename), log_("Configuration") {
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  std::fstream in(file_.c_str(), std::ios::in);
  if (!in.is_open()) {
	log_.oicprintf("file %s not found. Creating one...\n", filename.c_str());
	std::fstream out(file_.c_str(), std::ios::out | std::ios::trunc);
    if (!out.is_open()) throw std::exception();
	std::string json;
	pbjson::pb2json(this, json);
    out << prettify(json);
  } else {
	std::string json, err;
	std::getline(in, json, static_cast<char>(in.eof()));
    if (pbjson::json2pb(json, this, err) < 0) {
		log_.eicprintf(CL_RESET CL_RED "Error while parsing the file: %s" CL_RESET "\n", err.c_str());
      throw std::exception();
    }
  }
}

Config::~Config() {
  google::protobuf::ShutdownProtobufLibrary();
}

}
