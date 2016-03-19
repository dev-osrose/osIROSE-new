/*!
 * \file config.h
 *
 * \author L3nn0x
 * \date march 2016
 *
 * The configuration file
 */
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <spdlog/spdlog.h>
#include <string>
#include "configfile.pb.h"
#include "pbjson.hpp"
#include "logconsole.h"

#ifdef _WIN32
// this is only needed until we stop supporting visual studio 2013 and below
#define constexpr
#endif

namespace Core {

/*!
 * \class Config
 *
 * \brief The configuration class
 *
 * This class is used to fetch the configuration from a config file, or create it if it doesn't exist
 * The file is in \s Json format to make it easier to read and modify
 *
 * \note This class is a singleton because the configuration is the same across the entire server
 */
class Config : public configFile::Configuration {
 public:
  /*!
   * \brief Function to get the single instance of the Config class
   *
   * This function will create a \s Config instance with specified filename on first call. It'll afterwards just return the instance
   *
   * \param[in] filename The name of the configuration file to be loaded/created. It'll only be used on the fisrt call and thus can be ignored afterwards.
   * \param[out] Config& The \s Config instance
   */
  static Config &getInstance(std::string filename = "server.ini");


  struct Filename {
	  constexpr static const char *str() {
		  return "server.ini";
	  }
  };

 private:
  Config(std::string filename);
  Config(const Config &) = delete;
  Config &operator=(const Config &) = delete;
  ~Config();

  std::string file_;
  std::shared_ptr<spdlog::logger> logger_;
};

/*!
 * \brief A prettifying function for json formatting
 *
 * This function prettify a json table
 *
 * \param[in] std::string The json table to prettify
 * \param[out] std::string The json table prettified
 */
std::string prettify(const std::string &json);
}

#endif /* !_CONFIG_H_ */
