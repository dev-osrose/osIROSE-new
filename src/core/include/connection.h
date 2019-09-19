#pragma once

#include "connectionpool.h"
#include "osiroseDatabase.h"
#include <sqlpp11/sqlpp11.h>
#ifdef ENABLE_MYSQL
#  include <sqlpp11/mysql/connection.h>
#  include "mysqlconnection.h"
#endif

namespace Core {


#ifdef ENABLE_MYSQL
struct Osirose {
    using type = sqlpp::mysql::connection;
};

extern ConnectionPool<Osirose> &connectionPool;
#endif

using AccountTable = osiroseDatabase::Accounts;
using SessionTable = osiroseDatabase::Sessions;
using CharacterTable = osiroseDatabase::Characters;
using InventoryTable = osiroseDatabase::Inventory;
using SkillTable = osiroseDatabase::Skill;
using ItemDBTable = osiroseDatabase::ItemDb;
using PartyTable = osiroseDatabase::Party;
using PartyMembersTable = osiroseDatabase::PartyMembers;

std::string escapeData(const std::string &data);

}
