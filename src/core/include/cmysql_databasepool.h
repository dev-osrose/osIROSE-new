// Copyright 2016 Chirstopher Torres (Raven), L3nn0x
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http ://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * \file cmysql_databasepool
 *
 * \author L3nn0x
 * \date july 2016
 *
 * The implementation of \S IDatabasePool and \s IDatabasePoolFactory
 * \sa IDatabase
 */

#ifndef _CMYSQL_DATABASEPOOL_H_
#define _CMYSQL_DATABASEPOOL_H_

#include "idatabasepool.h"
#include "cmysql_database.h"
#include "config.h"

namespace Core {

/*!
 * \class CMySQL_DatabasePool
 * \brief The implementation of \s IDatabasePool that will use MySQL specific connections
 * \sa IDatabase CMySQL_Database
 *
 * This class is used to provide a simple implementation of a database pool.
 * It only contains one method that allow you to get a valid database connection.
 * This class can be used as is, but should be used by passing it as a template argument to \s CMySQL_DatabasePoolFactory
 *
 * \author L3nn0x
 * \date july 2016
 */

template <typename Filename>
class CMySQL_DatabasePool : public IDatabasePool {
	public:
		virtual IDatabase& getDatabase() {
			const ::configFile::Database    &dbb = Core::Config::getInstance(Filename::str()).database();
			static CMySQL_Database database(dbb.host(), dbb.database(), dbb.user(), dbb.password());
			return database;
		}
};

/*!
 * \class CMySQL_DatabasePool
 * \brief The implementation of \s IDatabasePoolFactory that will use MySQL specific database pool
 * \sa IDatabasePool CMySQL_DatabasePool
 *
 * This class is used to build the MySQL database pool. It doesn't do anything else.
 * This class should not be used as is but should be passed as a template argument to \s IDatabasePoolAbstractFactory
 *
 * \author L3nn0x
 * \date july 2016
 */

template <typename Filename>
class CMySQL_DatabasePoolFactory : public IDatabasePoolFactory {
	public:
		virtual IDatabasePool&	operator()() const {
			static CMySQL_DatabasePool<Filename> database_pool;
			return database_pool;
		}
};

}

#endif /* !_CMYSQL_DATABASEPOOL_H_ */
