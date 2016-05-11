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

	SET(BINDIR32_ENV_NAME "ProgramFiles(x86)")
	find_path( MYSQL_INCLUDE_DIR
		NAMES "mysql.h"
		PATHS "${EXTERNAL_INCLUDE_DIR}"
			  "C:/Program Files/MySQL/*/include"
			  "$ENV{PROGRAMFILES}/MySQL/*/include"
			  "$ENV{${BINDIR32_ENV_NAME}}/MySQL/*/include"
			  "$ENV{SYSTEMDRIVE}/MySQL/*/include" )
			  
	find_path( MYSQL_LIBRARY_PATH
		NAMES "libmysql.lib" "mysqlclient.lib" "mysqlclient_r.lib"
		PATHS "${EXTERNAL_BINARY_DIR}"
			  "C:/Program Files/MySQL/*/lib"
			  "$ENV{PROGRAMFILES}/MySQL/*/lib"
			  "$ENV{${BINDIR32_ENV_NAME}}/MySQL/*/lib"
			  "$ENV{SYSTEMDRIVE}/MySQL/*/lib" )
	
	find_library( MYSQL_LIBRARY
		NAMES "libmysql" "mysqlclient" "mysqlclient_r"
		PATHS "${EXTERNAL_BINARY_DIR}"
			  "C:/Program Files/MySQL/*/lib"
			  "$ENV{PROGRAMFILES}/MySQL/*/lib"
			  "$ENV{${BINDIR32_ENV_NAME}}/MySQL/*/lib"
			  "$ENV{SYSTEMDRIVE}/MySQL/*/lib" )
else()
	find_path( MYSQL_INCLUDE_DIR
		NAMES "mysql.h"
		PATHS "${EXTERNAL_INCLUDE_DIR}"
			  "/usr/include/mysql"
			  "/usr/local/include/mysql"
			  "/usr/mysql/include/mysql" )
			  
	find_path( MYSQL_LIBRARY_PATH
		NAMES "libmysql.a" "mysqlclient.a" "mysqlclient_r.a"
		PATHS "${EXTERNAL_BINARY_DIR}"
			  "/lib/mysql"
			  "/lib64/mysql"
			  "/usr/lib/mysql"
			  "/usr/lib64/mysql"
			  "/usr/local/lib/mysql"
			  "/usr/local/lib64/mysql"
			  "/usr/mysql/lib/mysql"
			  "/usr/mysql/lib64/mysql" )
	
	find_library( MYSQL_LIBRARY
		NAMES "libmysql" "mysqlclient" "mysqlclient_r"
		PATHS "${EXTERNAL_BINARY_DIR}"
			  "/lib/mysql"
			  "/lib64/mysql"
			  "/usr/lib/mysql"
			  "/usr/lib64/mysql"
			  "/usr/local/lib/mysql"
			  "/usr/local/lib64/mysql"
			  "/usr/mysql/lib/mysql"
			  "/usr/mysql/lib64/mysql" )
endif()



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

mark_as_advanced( MYSQL_INCLUDE_DIR MYSQL_LIBRARY MYSQL_LIBRARY_PATH )

