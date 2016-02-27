#ifndef _CMYSQL_DATABASE_H_
#define _CMYSQL_DATABASE_H_

#include "idatabase.h"
#include <mysql++.h>
#include "logconsole.h"

namespace Core {
class CMySQL_Result : public IResult {
 public:
  CMySQL_Result(mysqlpp::StoreQueryResult);
  virtual ~CMySQL_Result() {}

  virtual bool getString(std::string const &, std::string &data);
  virtual bool getInt(std::string const &, uint32_t &data);

 private:
  mysqlpp::StoreQueryResult res;
  template <typename T>
  bool getData(std::string const &name, T &data) {
    auto tmp = res[row][name.c_str()];
    if (tmp.is_null()) return false;
    data = static_cast<T>(tmp);
    return true;
  }
};

class CMySQL_Database : public IDatabase {
 public:
  CMySQL_Database();
  CMySQL_Database(std::string _host, std::string _database, std::string _user,
                  std::string _password);
  virtual ~CMySQL_Database();

  virtual void Connect(std::string _host, std::string _database,
                       std::string _user, std::string _password);

  virtual void QExecute(std::string _query);
  virtual IResult *QStore(std::string _query);

 private:
  std::string hostname;
  std::string database;
  std::string username;
  std::string password;

  mysqlpp::Connection conn;
  CLogConsole m_log;
  bool connected;
};
}

#endif
