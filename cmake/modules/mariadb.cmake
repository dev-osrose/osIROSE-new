message(STATUS "Looking for mariadb...")
if (WIN32)
  find_path(MYSQL_INCLUDE_DIR
      NAMES "mysql.h"
      PATHS "${CMAKE_THIRD_PARTY_INCLUDE_DIR}"
      "$ENV{PROGRAMFILES}/MariaDB/MariaDB Connector C 64-bit/include"
      "$ENV{PROGRAMFILES}/MariaDB/MariaDB 10.4/include/mysql"
      "$ENV{SYSTEMDRIVE}/MariaDB/MariaDB Connector C 64-bit/include"
      "$ENV{SYSTEMDRIVE}/MariaDB/include/mysql"
      )

  find_path(MYSQL_LIBRARY_PATH
      NAMES "libmysql.lib" "mysqlclient_r.lib" "libmariadb.lib" "mariadbclient.lib"
      PATHS "${CMAKE_THIRD_PARTY_LIBRARY_DIR}"
      "$ENV{PROGRAMFILES}/MariaDB/MariaDB Connector C 64-bit/lib"
      "$ENV{PROGRAMFILES}/MariaDB/MariaDB 10.4/lib"
      "$ENV{SYSTEMDRIVE}/MariaDB/MariaDB Connector C 64-bit/lib"
      "$ENV{SYSTEMDRIVE}/MariaDB/lib"
      )

  find_library(MYSQL_LIBRARY
      NAMES "libmysql" "mysqlclient_r" "libmariadb" "mariadbclient"
      PATHS "${CMAKE_THIRD_PARTY_LIBRARY_DIR}"
      "${MYSQL_LIBRARY_PATH}"
      )
else ()
  include(mysqldb)
endif ()

if (MYSQL_INCLUDE_DIR AND EXISTS "${MYSQL_INCLUDE_DIR}/mariadb_version.h")
  file(STRINGS "${MYSQL_INCLUDE_DIR}/mariadb_version.h"
      MYSQL_VERSION_H REGEX "^#define[ \t]+MYSQL_SERVER_VERSION[ \t]+\"[^\"]+\".*$")
  string(REGEX REPLACE
      "^.*MYSQL_SERVER_VERSION[ \t]+\"([^\"]+)\".*$" "\\1" MYSQL_VERSION_STRING
      "${MYSQL_VERSION_H}")
  else()
  message(STATUS "mariadb - not found")
endif ()