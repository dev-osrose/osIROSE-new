#include <string>
#include "cmysql_database.h"
#include <exception>
#include <stdexcept>

namespace Core {

CMySQL_Result::CMySQL_Result(mysqlpp::StoreQueryResult _res)
    : IResult(), res_(_res) {}

bool CMySQL_Result::getString(std::string const &name, std::string &data) {
  return getData<std::string>(name, data);
}

bool CMySQL_Result::getInt(std::string const &name, uint32_t &data) {
  return getData<uint32_t>(name, data);
}

CMySQL_Database::CMySQL_Database()
    : hostname_(""),
      database_(""),
      username_(""),
      password_(""),
      log_("MysqlDatabase"),
      connected_(false) {}

CMySQL_Database::CMySQL_Database(std::string _host, std::string _database,
                                 std::string _user, std::string _password)
    : hostname_(_host),
      database_(_database),
      username_(_user),
      password_(_password),
      log_("MysqlDatabase"),
      connected_(false) {
  try {
    conn_.connect(database_.c_str(), hostname_.c_str(), username_.c_str(),
                 password_.c_str());
    log_.icprintf("Connected to database\n");
  } catch (const std::exception &e) {
    log_.eicprintf(CL_RESET CL_RED
                    "Error while connecting to the database: %s" CL_RESET "\n",
                    conn_.error());
    throw e;
  }
  connected_ = true;
}

CMySQL_Database::~CMySQL_Database() {}

void CMySQL_Database::Connect(std::string _host, std::string _database,
                              std::string _user, std::string _password) {
  hostname_ = (_host);
  database_ = (_database);
  username_ = (_user);
  password_ = (_password);
  connected_ = false;

  try {
    conn_.connect(database_.c_str(), hostname_.c_str(), username_.c_str(),
                 password_.c_str());
  } catch (const std::exception &e) {
    log_.eicprintf(CL_RESET CL_RED
                    "Error while connecting to the database: %s" CL_RESET "\n",
                    conn_.error());
    throw e;
  }
  connected_ = true;
}

std::unique_ptr<IResult> CMySQL_Database::QStore(std::string _query) {
  if (!connected_) {
    log_.eicprintf(
        CL_RESET CL_RED
        "Error while executing the query '%s': not connected" CL_RESET "\n",
        _query.c_str());
    throw std::runtime_error("Error not connected");
  }
  std::lock_guard<std::mutex> lock(mutex_);
  log_.icprintf(CL_RESET "Executing query: %s\n", _query.c_str());
  mysqlpp::Query query = conn_.query(_query.c_str());
  return std::unique_ptr<IResult>(new CMySQL_Result(query.store()));
}

void CMySQL_Database::QExecute(std::string _query) {
  if (!connected_) {
    log_.eicprintf(
        CL_RESET CL_RED
        "Error while executing the query '%s': not connected" CL_RESET "\n",
        _query.c_str());
    throw std::runtime_error("Error not connected");
  }
  std::lock_guard<std::mutex> lock(mutex_);
  log_.icprintf(CL_RESET "Executing query: %s\n", _query.c_str());
  auto query = conn_.query(_query.c_str());
  query.exec(_query.c_str());
}
}
