#ifndef __CDATABASE_MANAGER_H__
#define __CDATABASE_MANAGER_H__
#include <memory>
#include "cmysql_database.h"

class CDatabaseMgr {
 public:
  static CDatabaseMgr& getInstance();
  static void deleteInstance();

  Core::CMySQL_Database& GetDatabase() { return *(database_.get()); }
  //Core::CMySQL_Database& GetDatabase() { return database_; }

 private:
  CDatabaseMgr();
  ~CDatabaseMgr();

  std::unique_ptr<Core::CMySQL_Database> database_;
  //Core::CMySQL_Database database_;
};

#endif
