#pragma once

#include "connectionpool.h"
#include "osiroseDatabase.h"
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/connection.h>
#include "mysqlconnection.h"

namespace Core {

const std::string osirose = "osirose";

auto &connectionPool = ConnectionPool<sqlpp::mysql::connection>::getInstance();

using AccountsTable = osiroseDatabase::Accounts;

std::string escapeData(const std::string &data);

}
