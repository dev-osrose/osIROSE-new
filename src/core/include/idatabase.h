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
 * \file idatabase.h
 *
 * \author L3nn0x
 * \date march 2016
 *
 * The interface definition for the database classes
 */

#ifndef _IDATABASE_H_
#define _IDATABASE_H_

#include <stdint.h>
#include <string>
#include <memory>
#include <vector>

namespace Core {
/*!
 * \class IRow
 *
 * \brief An interface for a single row result
 *
 * This class is used to represent a row from the results in memory. It has accessors to simplify its access
 * and ensure static typing.
 * \sa IResult IDatabase
 *
 * \author L3nn0x
 * \date march 2016
 */
class	IRow
{
	public:
		virtual ~IRow() {}

		/*!
		 * \brief Function to fetch a string
		 *
		 * This is a function to fetch a string from this row with specified column name
		 * This function returns false if the value was NULL. In this case it doesn't change the contents  of \s data
		 *
		 * \param[in] columnName The name of the column
		 * \param[out] data The resulting string
		 * \param[out] bool Return false if the data was NULL
		 */
		virtual	bool	getString(std::string const &columnName, std::string &data) = 0;

		/*!
		 * \brief Function to fetch a int
		 *
		 * This is a function to fetch a int from this row with specified column name
		 * This function returns false if the value was NULL. In this case it doesn't change the contents  of \s data
		 *
		 * \param[in] columnName The name of the column
		 * \param[out] data The resulting int
		 * \param[out] bool Return false if the data was NULL
		 */
        virtual bool    getInt(std::string const &columnName, uint64_t &data) = 0;
        virtual bool    getInt(std::string const &columnName, int64_t &data) = 0;
		virtual bool	getInt(std::string const &columnName, uint32_t &data) = 0;
        virtual bool    getInt(std::string const &columnName, int32_t &data) = 0;
        virtual bool    getInt(std::string const &columnName, uint16_t &data) = 0;
        virtual bool    getInt(std::string const &columnName, int16_t &data) = 0;
        virtual bool    getInt(std::string const &columnName, uint8_t &data) = 0;
        virtual bool    getInt(std::string const &columnName, int8_t &data) = 0;
        virtual bool    getInt(std::string const &columnName, bool &data) = 0;

		/*!
		 * \brief Function to fetch a float
		 *
		 * This is a function to fetch a float from this row with specified column name
		 * This function returns false if the value was NULL. In this case it doesn't change the contents  of \s data
		 *
		 * \param[in] columnName The name of the column
		 * \param[out] data The resulting float
		 * \param[out] bool Return false if the data was NULL
		 */
		virtual bool	getFloat(std::string const &columnName, float &data) = 0;
};

/*!
 * \class IResult
 *
 * \brief An interface for the result of a query
 *
 * This class is used to represent the results of a query in memory. It has accessors to simplify its access
 * and ensure static typing. It also acts as a container for all the \s IRow rows, allowing to iterate over it.
 * \sa IRow IDatabase
 *
 * \author L3nn0x
 * \date march 2016
 */
class	IResult
{
	public:
		typedef std::vector<std::unique_ptr<IRow>>::iterator		iterator;
		typedef std::vector<std::unique_ptr<IRow>>::const_iterator	const_iterator;

		IResult() : current_row_(0) {}
		virtual ~IResult() {}

		/*!
		 * \brief Convenience function to get results for a specific row
		 *
		 * This function is made to avoid \s IRow and just use \s IResult to get results. This function is used to specify the row that
		 * will be used when fetching the results with IResult member functions
		 * \sa getString getInt getFloat
		 *
		 * \param[in] _row The row that will be used by IResult getter functions
		 */
		virtual void		useRow(uint32_t _row) {current_row_ = _row;}

		/*!
		 * \brief Convenience function to increment the current row
		 *
		 * This function is made to avoid using \s IRow and increment the current row used by \s IResult to provide data from the getters
		 *
		 * \param[out] bool Return false if outside boundaries
		 * \note Checks only the upper bound
		 */
		virtual bool		incrementRow() {++current_row_; return true;}

		/*!
		 * \brief Convenience function to decrement the current row
		 *
		 * This function is made to avoid using \s IRow and decrement the current row used by \s IResult to provide data from the getters
		 *
		 * \param[out] bool Return false if outside boundaries
		 * \note Checks only the lower bound
		 */
		virtual bool		decrementRow() {uint32_t tmp = current_row_; current_row_ = current_row_ <= 0 ? 0 : current_row_ - 1; return tmp != 0;}

		/*!
		 * \brief Convenience function to get the total number of rows in this result
		 *
		 * This function returns the total number of rows that was returned by the query
		 *
		 */
		virtual uint32_t	size() const = 0;

		/*!
		 * \brief Convenience function to fetch a string
		 *
		 * This is a convenience function to fetch a string from the current row with specified column name
		 * This function returns false if the value was NULL. In this case it doesn't change the contents  of \s data
		 *
		 * \param[in] columnName The name of the column
		 * \param[out] data The resulting string
		 * \param[out] bool Return false if the data was NULL
		 */
		virtual	bool	getString(std::string const &columnName, std::string &data) = 0;

		/*!
		 * \brief Convenience function to fetch an int
		 *
		 * This is a convenience function to fetch a int from the current row with specified column name
		 * This function returns false if the value was NULL. In this case it doesn't change the contents  of \s data
		 *
		 * \param[in] columnName The name of the column
		 * \param[out] data The resulting int
		 * \param[out] bool Return false if the data was NULL
		 */
		virtual bool	getInt(std::string const &columnName, uint64_t &data) = 0;
		virtual bool	getInt(std::string const &columnName, int64_t &data) = 0;
		virtual bool	getInt(std::string const &columnName, uint32_t &data) = 0;
		virtual bool	getInt(std::string const &columnName, int32_t &data) = 0;
		virtual bool	getInt(std::string const &columnName, uint16_t &data) = 0;
		virtual bool	getInt(std::string const &columnName, int16_t &data) = 0;
		virtual bool	getInt(std::string const &columnName, uint8_t &data) = 0;
		virtual bool	getInt(std::string const &columnName, int8_t &data) = 0;
		virtual bool	getInt(std::string const &columnName, bool &data) = 0;

		/*!
		 * \brief Convenience function to fetch a float
		 *
		 * This is a convenience function to fetch a float from the current row with specified column name
		 * This function returns false if the value was NULL. In this case it doesn't change the contents  of \s data
		 *
		 * \param[in] columnName The name of the column
		 * \param[out] data The resulting float
		 * \param[out] bool Return false if the data was NULL
		 */
		virtual bool	getFloat(std::string const &columnName, float &data) = 0;

		virtual iterator	begin() {return rows_.begin();}
		virtual iterator	end() {return rows_.end();}
		virtual const_iterator	cbegin() const {return rows_.cbegin();}
		virtual const_iterator	cend() const {return rows_.cend();}

	protected:
		uint32_t				current_row_;
		std::vector<std::unique_ptr<IRow>>	rows_;
};

/*!
 * \class IDatabase
 *
 * \brief An interface for database access
 *
 * This class is used to represent a database accessor in memory. It is an interface to allow multiple database implementations. It returns \s IResult
 * as results of queries.
 * \sa IResult IRow
 *
 * \author L3nn0x
 * \date march 2016
 */
class IDatabase
{
public:
	virtual ~IDatabase() {}

	/*!
	 * \brief Used to connect to the database if it wasn't done in the constructor
	 *
	 * The connect function is used to connect to the database in case the constructor was called without arguments.
	 *
	 * \param[in] _host The host of the database
	 * \param[in] _database The name of the database to be used
	 * \param[in] _user The username used to connect to the database
	 * \param[in] _password The password used to connect to the database
	 * \note The connection should be initiated in the constructor as much as possible, this function is provided for convenience only.
	 * \warning If the database object was already connected when calling this function, the connection will be dropped and a new one will be made with the new parameters.
	 */
	virtual void	Connect(const std::string &_host, const std::string &_database, const std::string &_user, const std::string &_password) = 0;

	/*!
	 * \brief Used to make a query without result
	 *
	 * This function is used to make a query without wanting a result ( such as \s INSERT or \s DROP ) The function is thread safe.
	 *
	 * \param[in] _query The query to run against the database
	 * \note This function is blocking
	 */
	virtual void				QExecute(const std::string &_query) = 0;
	/*!
	 * \brief Used to make a query with results
	 *
	 * This function is used to make a query with a result ( such as \s SELECT ) The function is thread safe.
	 *
	 * \param[in] _query The query to run against the database
	 * \param[out] std::unique_ptr<IResult> The result of the query
	 * \note This function is blocking
	 */
	virtual std::unique_ptr<IResult>	QStore(const std::string &_query) = 0;
};
}

#endif
