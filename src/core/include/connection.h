#pragma once

#include "connectionpool.h"
#include "osiroseDatabase.h"
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/connection.h>

namespace Core {

const std::string osirose = "osirose";
const std::string log = "log";

auto &connectionPool = ConnectionPool<sqlpp::mysql::connection>::getInstance();

using AccountsTable = osiroseDatabase::Accounts;

std::string escapeData(const std::string &data);

}
