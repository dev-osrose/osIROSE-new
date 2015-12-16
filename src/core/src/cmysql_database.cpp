#include <string>
#include "cmysql_database.h"
#include <exception>
#include <stdexcept>

CMySQL_Database::CMySQL_Database(std::string _host, std::string _database, std::string _user, std::string _password) :
	hostname(_host), database(_database), username(_user), password(_password), m_log("MysqlDatabase"),
	connected(false)
{
	try {
		conn.connect(database.c_str(), hostname.c_str(), username.c_str(), password.c_str());
	} catch(const std::exception &e) {
		m_log.icprintf( CL_RESET CL_RED "Error while connecting to the database: %s" CL_RESET "\n", conn.error());
		throw e;
	}
	connected = true;
}

CMySQL_Database::~CMySQL_Database()
{}

void	CMySQL_Database::QExecute(std::string _query)
{
	if (!connected) {
		m_log.icprintf(CL_RESET CL_RED "Error while executing the query '%s': not connected" CL_RESET "\n", _query.c_str());
		throw std::runtime_error("Error not connected");
	}
	m_log.icprintf(CL_RESET "Executing query: %s\n", _query.c_str());
}


