#pragma once

#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/mysql.h>

#include <string>

namespace Core {

sqlpp::mysql::connection mysqlFactory(const std::string &user,
        const std::string &password,
        const std::string &database,
        const std::string &host);
}
