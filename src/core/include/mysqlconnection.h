#pragma once

#include <sqlpp23/mysql/mysql.h>

#include <memory>
#include <string>

namespace Core {

std::unique_ptr<sqlpp::mysql::connection> mysqlFactory(const std::string &user,
        const std::string &password,
        const std::string &database,
        const std::string &host,
        const unsigned short port);
}
