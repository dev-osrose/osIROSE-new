#include "config.h"
#include <fstream>
#include <iostream>
#include <exception>

using namespace std;
using namespace pbjson;

Config	&Config::getInstance(std::string filename)
{
	static Config	instance(filename);
	return instance;
}

Config::Config(string filename) : Configuration(), file(filename)
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	if (filename == "")
		file = "server.ini";
	fstream	in(file.c_str(), ios::in);
	if (!in.is_open()) {
		cout << file << " file not found. Creating one" << endl;
		fstream	out(file.c_str(), ios::out | ios::trunc);
		string	json;
		pb2json(this, json);
		out << json;
	} else {
		string	json, err;
		getline(in, json, static_cast<char>(in.eof()));
		if (json2pb(json, this, err) < 0) {
			cerr << "Error while parsing the file: " << err << endl;
			throw std::exception();
		}
	}
}

Config::~Config()
{
	/* fstream	out(file.c_str(), ios::out | ios::trunc); */
	/* string	json; */
	/* pb2json(this, json); */
	/* out << json; */
	google::protobuf::ShutdownProtobufLibrary();
}
