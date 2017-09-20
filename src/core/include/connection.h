#pragma once

#include "connectionpool.h"
#include "osiroseDatabase.h"
#include <sqlpp11/sqlpp11.h>
#ifdef ENABLE_MYSQL
#  include <sqlpp11/mysql/connection.h>
#  include "mysqlconnection.h"
#endif

namespace Core {

const std::string osirose = "osirose";

#ifdef ENABLE_MYSQL
extern ConnectionPool<sqlpp::mysql::connection> &connectionPool;
#endif

using AccountTable = osiroseDatabase::Accounts;
using SessionTable = osiroseDatabase::Sessions;
using CharacterTable = osiroseDatabase::Characters;
using InventoryTable = osiroseDatabase::Inventory;
using SkillTable = osiroseDatabase::Skill;
<<<<<<< HEAD
using ItemDBTable = osiroseDatabase::ItemDb;
=======
 using WishTable = osiroseDatabase::Wishlist;
>>>>>>> origin/trunk

std::string escapeData(const std::string &data);

}
