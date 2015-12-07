#include <string>
#include "cmysql_database.h"

CMySQL_Database::CMySQL_Database(std::string _host, std::string _database, std::string _user, std::string _password)
{
	hostname = _host;
	database = _database;
	username = _user;
	password = _password;
}

CMySQL_Database::~CMySQL_Database()
{
}

bool CMySQL_Database::Connect()
{
	return true;
}

void CMySQL_Database::Disconnect()
{
}

bool CMySQL_Database::QExecute(std::string _query)
{
	(void)_query;
	return true;
}


