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

#include <string>
#include "cmysql_database.h"
#include <exception>
#include <stdexcept>
#include "database.h"
#include <dbdriver.h>

namespace Core {

#ifdef USE_MYSQL_ONE_INSTANCE
IDatabasePool &databasePool = databasePoolFilename<Config::Filename>::getInstance();
#endif

bool CMySQL_Row::getString(std::string const &name, std::string &data) {
  return getData<std::string>(name, data);
}

bool CMySQL_Row::getInt(std::string const &name, uint32_t &data) {
  return getData<uint32_t>(name, data);
}

bool CMySQL_Row::getFloat(std::string const &name, float &data) {
  return getData<float>(name, data);
}

CMySQL_Result::CMySQL_Result(const mysqlpp::StoreQueryResult &_res)
    : IResult() {
  for (auto const &it : _res)
    rows_.emplace_back(std::unique_ptr<IRow>(new CMySQL_Row(it)));
}

bool CMySQL_Result::incrementRow() {
  uint32_t tmp = current_row_;
  current_row_ =
      current_row_ >= rows_.size() ? rows_.size() - 1 : current_row_ + 1;
  return tmp == rows_.size();
}

bool CMySQL_Result::getString(std::string const &name, std::string &data) {
  return rows_[current_row_]->getString(name, data);
}

bool CMySQL_Result::getInt(std::string const &name, uint32_t &data) {
  return rows_[current_row_]->getInt(name, data);
}

bool CMySQL_Result::getFloat(std::string const &name, float &data) {
  return rows_[current_row_]->getFloat(name, data);
}

CMySQL_Database::CMySQL_Database()
    : hostname_(""),
      database_(""),
      username_(""),
      password_(""),
      connected_(false) {
  logger_ = CLog::GetLogger(log_type::DATABASE);
}

CMySQL_Database::~CMySQL_Database() {
  if (auto log = logger_.lock())
  {
    log->trace("CMySQL_Database destructor called!");
    log->debug( "db shared_ptr used by {}", log.use_count() - 1);
  }
  logger_.reset();
}

CMySQL_Database::CMySQL_Database(const std::string &_host, const std::string &_database,
                                 const std::string &_user, const std::string &_password) {
	Connect(_host, _database, _user, _password);
}

void CMySQL_Database::Connect(const std::string &_host, const std::string &_database,
                              const std::string &_user, const std::string &_password) {
  hostname_ = (_host);
  database_ = (_database);
  username_ = (_user);
  password_ = (_password);
  connected_ = false;
  logger_ = CLog::GetLogger(log_type::DATABASE);

  try {
    conn_.set_option(new mysqlpp::MultiStatementsOption(true));
    conn_.connect(database_.c_str(), hostname_.c_str(), username_.c_str(),
                  password_.c_str());
  } catch (const std::exception &e) {
    if (auto log = logger_.lock())
      log->critical( "Error while connecting to the database: {}", conn_.error());
    throw e;
  }
  connected_ = true;
}

std::unique_ptr<IResult> CMySQL_Database::QStore(const std::string &_query) {
  if (!connected_) {
    if (auto log = logger_.lock())
      log->critical() << Color::FG_RED
                      << "Error while executing the query: " << _query.c_str()
                      << ": not connected" << Color::CL_RESET;
    throw std::runtime_error("Error not connected");
  }
  std::lock_guard<std::mutex> lock(mutex_);
  conn_.thread_start();  // This is if we pass the database around different
                         // threads
  if (auto log = logger_.lock())
    log->debug() << "Executing query: " << _query.c_str();
  mysqlpp::Query query = conn_.query(_query.c_str());
  std::unique_ptr<IResult> result(new CMySQL_Result(query.store()));
  while (query.more_results()) {
    auto tmp = query.store_next();
  }
  return result;
}

void CMySQL_Database::QExecute(const std::string &_query) {
  if (!connected_) {
    if (auto log = logger_.lock())
      log->critical() << Color::FG_RED
                      << "Error while executing the query: " << _query.c_str()
                      << ": not connected" << Color::CL_RESET;
    throw std::runtime_error("Error not connected");
  }
  std::lock_guard<std::mutex> lock(mutex_);
  conn_.thread_start();  // This is if we pass the database around different
                         // threads
  if (auto log = logger_.lock())
    log->debug() << "Executing query: " << _query.c_str();
  auto query = conn_.query(_query.c_str());
  query.exec(_query.c_str());
}

static bool replaceAll(std::string &str, const std::string &from, const std::string &to) {
	size_t pos = 0;
	bool isFound = false;
	while ((pos = str.find(from, pos)) != std::string::npos) {
		str.replace(pos, from.length(), to);
		pos += to.length();
		isFound = true;
	}
	return isFound;
}

std::string CMySQL_Database::escapeData(const std::string &data) {
  std::string result(data);
  replaceAll(result, "'", "\\'");
  replaceAll(result, "\"", "\\\"");
  replaceAll(result, "%", "\\%");
  replaceAll(result, "_", "\\_");
  replaceAll(result, ";", "");
  return result;
}

}
