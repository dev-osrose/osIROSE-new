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


#ifndef _CMYSQL_DATABASEPOOL_H_
#define _CMYSQL_DATABASEPOOL_H_

#include "idatabasepool.h"
#include "cmysql_database.h"
#include "config.h"

namespace Core {

template <typename Filename>
class CMySQL_DatabasePool : public IDatabasePool {
	public:
		virtual IDatabase& getDatabase() {
			const ::configFile::Database    &dbb = Core::Config::getInstance(Filename::str()).database();
			static CMySQL_Database database(dbb.host(), dbb.database(), dbb.user(), dbb.password());
			return database;
		}
};

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
