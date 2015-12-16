#ifndef _CMYSQL_DATABASE_H_
#define _CMYSQL_DATABASE_H_

#include "idatabase.h"
#include <mysql++.h>
#include "logconsole.h"

class CMySQL_Database : public IDatabase
{
public:
	CMySQL_Database(std::string _host, std::string _database, std::string _user, std::string _password);
	virtual ~CMySQL_Database();

	virtual void	QExecute(std::string _query);
private:
	std::string hostname;
	std::string database;
	std::string username;
	std::string password;

	mysqlpp::Connection	conn;
	CLogConsole m_log;
	bool		connected;
};

#endif
