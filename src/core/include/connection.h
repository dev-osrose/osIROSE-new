#pragma once

#include "connectionpool.h"
#include "osiroseDatabase.h"
#include <sqlpp23/sqlpp23.h>
#if defined(ENABLE_MYSQL) || defined(ENABLE_MARIADB)
#  include <sqlpp23/mysql/mysql.h>
#  include "mysqlconnection.h"
#endif


namespace Core {


#if defined(ENABLE_MYSQL) || defined(ENABLE_MARIADB)
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
