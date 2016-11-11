#pragma once

#include "idatabasepool.h"

template <class T>
class MockDatabasePool : public IDatabasePool {
	public:
		virtual IDatabase& getDatabase() {
			static T database;
			return database;
		}
};

template <class T>
class MockDatabasePoolFactory : public IDatabasePoolFactory {
	public:
		virtual IDatabasePool&	operator()() const {
			static MockDatabasePool<T> database_pool;
			return database_pool;
		}
};

template <class T>
using databasePoolT = IDatabasePoolAbstractFactory<MockDatabasePoolFactory<T>>;

extern IDatabasePool &databasePool;

// to put in a cpp file IDatabasePool &databasePool = databasePoolT<MockDatabase>::getInstance();
