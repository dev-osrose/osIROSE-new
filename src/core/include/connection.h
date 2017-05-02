#pragma once

#include "connectionpool.h"
#include "osiroseDatabase.h"
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/connection.h>
#include "mysqlconnection.h"

namespace Core {

auto &connectionPool = ConnectionPool<sqlpp::mysql::connection>::getInstance();

constexpr std::size_t osirose = std::remove_reference_t<decltype(connectionPool)>::getId(mysqlFactory);

using AccountsTable = osiroseDatabase::Accounts;

std::string escapeData(const std::string &data);

}
