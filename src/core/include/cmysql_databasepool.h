
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
