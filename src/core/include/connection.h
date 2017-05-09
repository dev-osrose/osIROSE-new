#pragma once

#include "connectionpool.h"
#include "osiroseDatabase.h"
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/connection.h>
#include "mysqlconnection.h"

namespace Core {

const std::string osirose = "osirose";

extern ConnectionPool<sqlpp::mysql::connection> &connectionPool;

using AccountTable = osiroseDatabase::Accounts;
using SessionTable = osiroseDatabase::Sessions;
using CharacterTable = osiroseDatabase::Characters;
using InventoryTable = osiroseDatabase::Inventory;
using SkillTable = osiroseDatabase::Skill;

std::string escapeData(const std::string &data);

}
