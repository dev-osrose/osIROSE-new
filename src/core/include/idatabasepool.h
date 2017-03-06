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
 * \file idatabasepool.h
 *
 * \author L3nn0x
 * \date march 2016
 *
 * This file holds the IDatabasePool and associated singleton.
 */
#ifndef _IDATABASEPOOL_H_
#define _IDATABASEPOOL_H_

#include "idatabase.h"

namespace Core {

/*!
 * \class IDatabasePool
 *
 * \brief The interface for the database pool class.
 *
 * This class is created by \s IDatabasePoolFactory. It will always be what you get from \s databasePool::getInstance()
 * \sa CMySQL_DatabasePool IdatabasePoolFactory CMySQL_DatabasePoolFactory IDatabasePoolAbstractFactory CMySQL_DatabasePoolAbstractFactory
 * \sa IDatabaseFactory databasePool databasePoolFilename
 *
 * \author L3nn0x
 * \date march 2016
 */
class IDatabasePool {
	public:
		/*!
		 * \brief This function returns a connected database from the correct implementation
		 *
		 * The pool gets automatically the configuration file and handles it internally
		 * \sa IDatabase CMySQL_Database
		 *
		 * \param[out] IDatabase& A reference to a live database connection
		 *
		 * \note Please see \s CMySQL_DatabasePool for an example of the implementation
		 * \throw std::runtime_error If the database connection couldn't be made
		 */
		virtual IDatabase&	getDatabase() = 0;
};

/*!
 * \class IDatabaseFactory
 *
 * \brief The factory responsible for creating the correct \s IDatabasePool
 *
 * This class is used by \s IDatabasePoolAbstractFactory. You will never have to use it except if you are creating a new
 * \s IDatabasePool subclass. In that case you'll have to subclass this factory.
 * \sa CMySQL_DatabasePoolFactory IDatabasePoolAbstractFactory
 * \sa IDatabasePool
 *
 * \author L3nn0x
 * \date march 2016
 */
class IDatabasePoolFactory {
	public:
		/*!
		 * \brief This function construct and returns the correct \s IDatabasePool implementation
		 *
		 * This functions is only necessary for \s IDatabasePoolAbstractFactory
		 *
		 * \param[out] IDatabasePool& A valid reference to the database pool
		 *
		 * \note Please see \s CMySQL_DatabasePoolFactory for an example of the implementation
		 */
		virtual IDatabasePool&	operator()() const = 0;
};

/*!
 * \class IDatabasePoolAbstractFactory
 *
 * \brief The factory responsible for creating the correct \s IDatabaseFactory
 *
 * This class is the singleton you are calling. You get an \s IDatabasePool instance
 * You wont have to manipulate this class directly, just use the \s databasePool typedef instead.
 * \sa CMySQL_DatabasePoolFactory databasePool databasePoolFilename
 *
 * \author L3nn0x
 * \date march 2016
 */
template <typename Factory>
class IDatabasePoolAbstractFactory {
	public:
		/*!
		 * \brief This function construct and returns the correct \s IDatabase pool singleton implementation
		 *
		 * The initialisation is done the first time getInstance is called.
		 *
		 * \param[out] IDatabasePool& A valid reference to the database pool singleton
		 *
		 * \note This function will call the oerator () of the Factory template thus the \s databasePoolFilename typedef
		 */
		static IDatabasePool& getInstance() {
			static IDatabasePool &pool = Factory()();
			return pool;
		}

	private:
		IDatabasePoolAbstractFactory() = delete;
		IDatabasePoolAbstractFactory(const IDatabasePoolAbstractFactory&) = delete;
		IDatabasePoolAbstractFactory& operator=(const IDatabasePoolAbstractFactory&) = delete;
};

}

#endif /* !_IDATABASEPOOL_H_ */
