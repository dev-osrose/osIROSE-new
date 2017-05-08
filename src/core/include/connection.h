#pragma once

#include "connectionpool.h"
#include "osiroseDatabase.h"
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/connection.h>
#include "mysqlconnection.h"

namespace Core {

const std::string osirose = "osirose";

extern ConnectionPool<sqlpp::mysql::connection> &connectionPool;

using AccountsTable = osiroseDatabase::Accounts;

std::string escapeData(const std::string &data);

}
