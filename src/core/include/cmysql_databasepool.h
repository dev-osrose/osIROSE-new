
#ifndef _CMYSQL_DATABASEPOOL_H_
#define _CMYSQL_DATABASEPOOL_H_

#include "idatabasepool.h"
#include "cmysql_database.h"
#include "config.h"

// !!!!!!!!!!!!!!!!!!!! THIS SHOULD BE DEFINED AT COMPILE TIME !!!!!!!!!!!!!!!!!!
#ifndef USE_MYSQL
#define USE_MYSQL
#endif

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

// you will need to define USE_MYSQL at compile time (g++ -DUSE_MYSQL) to use mysql
#ifdef USE_MYSQL
	template <typename Filename>
	using databasePool = IDatabasePoolAbstractFactory<CMySQL_DatabasePoolFactory<Filename>>;
#endif
}

#endif /* !_CMYSQL_DATABASEPOOL_H_ */
