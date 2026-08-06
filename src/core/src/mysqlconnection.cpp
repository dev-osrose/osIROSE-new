#include "mysqlconnection.h"

std::unique_ptr<sqlpp::mysql::connection> Core::mysqlFactory(const std::string &user,
        const std::string &password,
        const std::string &database,
        const std::string &host,
        const unsigned short port) {
    // Idempotent: calls mysql_library_init once and registers mysql_library_end
    // for shutdown, no matter how often we end up here.
    sqlpp::mysql::global_library_init();

    auto config = std::make_shared<sqlpp::mysql::connection_config>();
    config->user = user;
    config->password = password;
    config->database = database;
    config->host = host;
    config->port = port;
    return std::make_unique<sqlpp::mysql::connection>(config);
}
