/*!
 * \file database.h
 *
 * This file is an aggregator for all the different databases/pools we can have
 * Everything is dependent on a few compiler time setup defines
 *
 * If you define \s USE_MYSQL_ONE_INSTANCE with your compiler then it will use one instance
 * MySQL connection
 *
 * \author L3nn0x
 * \date march 2016
 */
#ifndef __DATABASE__
#define __DATABASE__

#include "cmysql_databasepool.h"
#include "config.h"

namespace Core {

#ifdef USE_MYSQL_ONE_INSTANCE
/*!
 * \var databasePoolFilename
 *
 * brief a typedef provided for syntaxic sugar. It needs a template parameter for the filename.
 *
 * Usage : auto &database = Core::databasePoolFilename<Filename>::getInstance().getDatabase(); with Filename declared
 * as follow : struct Filename { constexpr static const char *str() { return "your filename"; } };
 * The default filename is "server.ini", you might want to see \s databasePool
 * \sa databasePool
 *
 */
template <typename Filename>
using databasePoolFilename = IDatabasePoolAbstractFactory<CMySQL_DatabasePoolFactory<Filename>>;
/*!
 * \var databasePool
 *
 * brief a typedef provided for syntaxic sugar.
 *
 * Usage : auto &database = Core::databasePool::getInstance().getDatabase();
 * The default filename "server.ini" will be used by this typedef.
 * \sa databasePoolFilename
 *
 */
extern IDatabasePool &databasePool;
#endif

}

#endif
