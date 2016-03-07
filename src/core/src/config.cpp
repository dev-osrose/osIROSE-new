#include "config.h"
#include <fstream>
#include <iostream>
#include <exception>

using namespace std;
using namespace pbjson;

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

Config::Config(string filename) : Configuration(), file(filename) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  fstream in(file.c_str(), ios::in);
  if (!in.is_open()) {
    cout << file << " file not found. Creating one" << endl;
    fstream out(file.c_str(), ios::out | ios::trunc);
    if (!out.is_open()) throw std::exception();
    string json;
    pb2json(this, json);
    out << prettify(json);
  } else {
    string json, err;
    getline(in, json, static_cast<char>(in.eof()));
    if (json2pb(json, this, err) < 0) {
      cerr << "Error while parsing the file: " << err << endl;
      throw std::exception();
    }
  }
}

Config::~Config() {
  /* fstream	out(file.c_str(), ios::out | ios::trunc); */
  /* string	json; */
  /* pb2json(this, json); */
  /* out << json; */
  google::protobuf::ShutdownProtobufLibrary();
}
