#include <string>
#include "cmysql_database.h"
#include <exception>
#include <stdexcept>

CMySQL_Result::CMySQL_Result(mysqlpp::StoreQueryResult _res) : IResult(), res(_res)
{}

bool	CMySQL_Result::getString(std::string const &name, std::string &data)
{
	return getData<std::string>(name, data);
}

bool	CMySQL_Result::getInt(std::string const &name, uint32_t &data)
{
	return getData<uint32_t>(name, data);
}

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

IResult	*CMySQL_Database::QStore(std::string _query)
{
	if (!connected) {
		m_log.icprintf(CL_RESET CL_RED "Error while executing the query '%s': not connected" CL_RESET "\n", _query.c_str());
		throw std::runtime_error("Error not connected");
	}
	m_log.icprintf(CL_RESET "Executing query: %s\n", _query.c_str());
	mysqlpp::Query	query = conn.query(_query.c_str());
	CMySQL_Result	*res = new CMySQL_Result(query.store());
	return res;
}


void	CMySQL_Database::QExecute(std::string _query)
{
	if (!connected) {
		m_log.icprintf(CL_RESET CL_RED "Error while executing the query '%s': not connected" CL_RESET "\n", _query.c_str());
		throw std::runtime_error("Error not connected");
	}
	m_log.icprintf(CL_RESET "Executing query: %s\n", _query.c_str());
	auto	query = conn.query(_query.c_str());
	query.exec(_query.c_str());
}
