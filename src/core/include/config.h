#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include "configfile.pb.h"
#include "pbjson.hpp"

class	Config : public configFile::Configuration
{
	public:
		Config(std::string filename);
		~Config();

	private:
		std::string	file;
};

#endif /* !_CONFIG_H_ */
