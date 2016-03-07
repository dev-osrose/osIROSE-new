#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include "configfile.pb.h"
#include "pbjson.hpp"
#include "logconsole.h"

namespace Core {

class Config : public configFile::Configuration {
 public:
  static Config &getInstance(std::string filename = "server.ini");

  static std::string prettify(std::string);

 private:
  Config(std::string filename);
  Config(const Config &) = delete;
  Config &operator=(const Config &) = delete;
  ~Config();

  std::string file_;
  CLogConsole log_;
};

}

#endif /* !_CONFIG_H_ */
