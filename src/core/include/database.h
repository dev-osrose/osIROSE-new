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
template <typename Filename>
using databasePoolFilename = IDatabasePoolAbstractFactory<CMySQL_DatabasePoolFactory<Filename>>;
using databasePool = databasePoolFilename<CMySQL_DatabasePoolFactory<Config::Filename>;
#endif

}

#endif
