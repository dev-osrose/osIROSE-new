# - Try to find MySQL.
# Once done this will define:
# MYSQL_FOUND			- If false, do not try to use MySQL.
# MYSQL_INCLUDE_DIRS	- Where to find mysql.h, etc.
# MYSQL_LIBRARIES		- The libraries to link against.
# MYSQL_VERSION_STRING	- Version in a string of MySQL.
#
# Created by RenatoUtsch based on eAthena implementation.
#
# Please note that this module only supports Windows and Linux officially, but
# should work on all UNIX-like operational systems too.
#

#=============================================================================
# Copyright 2012 RenatoUtsch
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

if( WIN32 )

	SET(BINDIR32_ENV_NAME "Program Files (x86)")
	find_path( MYSQL_INCLUDE_DIR
		NAMES "mysql.h"
		PATHS "${CMAKE_THIRD_PARTY_INCLUDE_DIR}"
			  "C:/Program Files/MySQL/MySQL Connector.C 6.1/include"
			  "C:/${BINDIR32_ENV_NAME}/MySQL/MySQL Connector.C 6.1/include"
			  "$ENV{SYSTEMDRIVE}/MySQL/MySQL Connector.C 6.1/include"
			  "$ENV{PROGRAMFILES}/MySQL/*/include"
			  "C:/${BINDIR32_ENV_NAME}/MySQL/*/include"
			  "$ENV{SYSTEMDRIVE}/MySQL/*/include"
			  "C:/Program Files/MySQL/*/include"
			  )

	find_path( MYSQL_LIBRARY_PATH
		NAMES "libmysql.lib" "mysqlclient_r.lib"
		PATHS "${EXTERNAL_BINARY_DIR}"
			  "C:/Program Files/MySQL/MySQL Connector.C 6.1/lib"
			  "C:/${BINDIR32_ENV_NAME}/MySQL/MySQL Connector.C 6.1/lib"
			  "$ENV{SYSTEMDRIVE}/MySQL/MySQL Connector.C 6.1/lib"
			  "$ENV{PROGRAMFILES}/MySQL/*/lib"
			  "C:/${BINDIR32_ENV_NAME}/MySQL/*/lib"
			  "$ENV{SYSTEMDRIVE}/MySQL/*/lib"
			  "C:/Program Files/MySQL/*/lib"
			  )

	find_library( MYSQL_LIBRARY
		NAMES "libmysql" "mysqlclient_r"
		PATHS "${EXTERNAL_BINARY_DIR}"
			  "C:/Program Files/MySQL/MySQL Connector.C 6.1/lib"
			  "C:/${BINDIR32_ENV_NAME}/MySQL/MySQL Connector.C 6.1/lib"
			  "$ENV{SYSTEMDRIVE}/MySQL/MySQL Connector.C 6.1/lib"
			  "$ENV{PROGRAMFILES}/MySQL/*/lib"
			  "C:/${BINDIR32_ENV_NAME}/MySQL/*/lib"
			  "$ENV{SYSTEMDRIVE}/MySQL/*/lib"
			  "C:/Program Files/MySQL/*/lib"
			  )
else()
	find_path( MYSQL_INCLUDE_DIR
		NAMES "mysql.h"
		PATHS "${CMAKE_THIRD_PARTY_INCLUDE_DIR}"
			  "$ENV{HOME}/mysql/include"
			  "/usr/include/mysql"
			  "/usr/local/include/mysql"
			  "/usr/mysql/include/mysql" )

	find_path( MYSQL_LIBRARY_PATH
		NAMES libmysqlclient.a libmysqlclient.so libmysqlclient_r.a libmysqlclient_r.so
		PATHS "${CMAKE_THIRD_PARTY_DIR}"
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
              "/usr/lib/x86_64-linux-gnu" )

	find_library( MYSQL_LIBRARY
		NAMES mysqlclient mysqlclient_r
		PATHS "${CMAKE_THIRD_PARTY_DIR}"
			  "$ENV{HOME}/mysql/lib"
			  "/lib/mysql"
			  "/lib64/mysql"
              "/usr/lib"
			  "/usr/lib/mysql"
			  "/usr/lib64/mysql"
			  "/usr/local/lib/mysql"
			  "/usr/local/lib64/mysql"
			  "/usr/mysql/lib/mysql"
			  "/usr/mysql/lib64/mysql" )
endif()

MESSAGE(STATUS "MySQL Library: ${MYSQL_LIBRARY}")
MESSAGE(STATUS "MySQL Library Path: ${MYSQL_LIBRARY_PATH}")
MESSAGE(STATUS "MySQL Include Path: ${MYSQL_INCLUDE_DIR}")

if( MYSQL_INCLUDE_DIR AND EXISTS "${MYSQL_INCLUDE_DIR}/mysql_version.h" )
	file( STRINGS "${MYSQL_INCLUDE_DIR}/mysql_version.h"
		MYSQL_VERSION_H REGEX "^#define[ \t]+MYSQL_SERVER_VERSION[ \t]+\"[^\"]+\".*$" )
	string( REGEX REPLACE
		"^.*MYSQL_SERVER_VERSION[ \t]+\"([^\"]+)\".*$" "\\1" MYSQL_VERSION_STRING
		"${MYSQL_VERSION_H}" )
endif()

# handle the QUIETLY and REQUIRED arguments and set MYSQL_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( MYSQL DEFAULT_MSG
	MYSQL_LIBRARY MYSQL_LIBRARY_PATH MYSQL_INCLUDE_DIR
	MYSQL_VERSION_STRING )

set( MYSQL_INCLUDE_DIRS ${MYSQL_INCLUDE_DIR} )
set( MYSQL_LIBRARIES ${MYSQL_LIBRARY} )

if(MYSQL_FOUND AND NOT TARGET mysql::mysql)
    add_library(mysql::mysql UNKNOWN IMPORTED)
    set_target_properties(mysql::mysql PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${MYSQL_INCLUDE_DIR}")
    set_property(TARGET mysql::mysql APPEND PROPERTY IMPORTED_LOCATION "${MYSQL_LIBRARY}")
endif()

mark_as_advanced( MYSQL_INCLUDE_DIR MYSQL_LIBRARY MYSQL_LIBRARY_PATH )

