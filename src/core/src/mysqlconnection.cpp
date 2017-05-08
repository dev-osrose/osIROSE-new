#include "mysqlconnection.h"

using namespace Core;

std::unique_ptr<sqlpp::mysql::connection> mysqlFactory(const std::string &user,
        const std::string &password,
        const std::string &database,
        const std::string &host) {
    auto config = std::make_shared<sqlpp::mysql::connection_config>();
    config->user = user;
    config->password = password;
    config->database = database;
    config->host = host;
    return std::make_unique<sqlpp::mysql::connection>(config);
}
