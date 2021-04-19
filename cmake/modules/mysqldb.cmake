message(STATUS "Looking for mysql...")
if (WIN32)
  find_path(MYSQL_INCLUDE_DIR
      NAMES "mysql.h"
      PATHS "${CMAKE_THIRD_PARTY_INCLUDE_DIR}"
      "C:/Program Files/MySQL/MySQL Connector.C */include"
      "$ENV{SYSTEMDRIVE}/MySQL/MySQL Connector.C */include"
      "$ENV{PROGRAMFILES}/MySQL/*/include"
      "$ENV{SYSTEMDRIVE}/MySQL/*/include"
      "C:/Program Files/MySQL/*/include"
      )

  find_path(MYSQL_LIBRARY_PATH
      NAMES "libmysql.lib" "mysqlclient_r.lib" "libmariadb.lib" "mariadbclient.lib"
      PATHS "${CMAKE_THIRD_PARTY_LIBRARY_DIR}"
      "${MYSQL_INCLUDE_DIR}/../lib"
      "$ENV{SYSTEMDRIVE}/MySQL/MySQL Connector.C */lib"
      "$ENV{PROGRAMFILES}/MySQL/*/lib"
      "$ENV{SYSTEMDRIVE}/MySQL/*/lib"
      "C:/Program Files/MySQL/*/lib"
      )

  find_library(MYSQL_LIBRARY
      NAMES "libmysql" "mysqlclient_r" "libmariadb" "mariadbclient"
      PATHS "${CMAKE_THIRD_PARTY_LIBRARY_DIR}"
      "${MYSQL_LIBRARY_PATH}"
      )
else ()
  find_path(MYSQL_INCLUDE_DIR
      NAMES "mysql.h"
      PATHS "${CMAKE_THIRD_PARTY_INCLUDE_DIR}"
      "$ENV{HOME}/mysql/include"
      "/usr/include/mysql"
      "/usr/local/include/mysql"
      "/usr/mysql/include/mysql")

  find_path(MYSQL_LIBRARY_PATH
      NAMES libmysqlclient.a libmysqlclient.so libmysqlclient_r.a libmysqlclient_r.so
      PATHS "${CMAKE_THIRD_PARTY_LIBRARY_DIR}"
      "$ENV{HOME}/mysql/lib"
      "/lib/mysql"
      "/lib64/mysql"
      "/usr/lib"
      "/usr/lib/mysql"
      "/usr/lib64/mysql"
      "/usr/local/lib/mysql"
      "/usr/local/lib64/mysql"
      "/usr/mysql/lib/mysql"
      "/usr/mysql/lib64/mysql"
      "/usr/lib/x86_64-linux-gnu")

  find_library(MYSQL_LIBRARY
      NAMES mysqlclient mysqlclient_r
      PATHS "${CMAKE_THIRD_PARTY_LIBRARY_DIR}"
      "$ENV{HOME}/mysql/lib"
      "/lib/mysql"
      "/lib64/mysql"
      "/usr/lib"
      "/usr/lib/mysql"
      "/usr/lib64/mysql"
      "/usr/local/lib/mysql"
      "/usr/local/lib64/mysql"
      "/usr/mysql/lib/mysql"
      "/usr/mysql/lib64/mysql")
endif ()

if (MYSQL_INCLUDE_DIR AND EXISTS "${MYSQL_INCLUDE_DIR}/mysql_version.h")
  file(STRINGS "${MYSQL_INCLUDE_DIR}/mysql_version.h"
      MYSQL_VERSION_H REGEX "^#define[ \t]+MYSQL_SERVER_VERSION[ \t]+\"[^\"]+\".*$")
  string(REGEX REPLACE
      "^.*MYSQL_SERVER_VERSION[ \t]+\"([^\"]+)\".*$" "\\1" MYSQL_VERSION_STRING
      "${MYSQL_VERSION_H}")
else()
  message(STATUS "mysql - not found")
endif ()