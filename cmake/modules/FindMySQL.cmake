# - Try to find MySQL.
# Once done this will define:
# MySQL_FOUND			- If false, do not try to use MySQL.
# MYSQL_INCLUDE_DIRS	- Where to find mysql.h, etc.
# MYSQL_LIBRARIES		- The libraries to link against.
# MYSQL_VERSION_STRING	- Version in a string of MySQL.

if (MySQL_FOUND)
  # Already in cache, be silent
  set(MYSQL_FIND_QUIETLY TRUE)
endif ()

include(mariadb)
if (NOT MYSQL_INCLUDE_DIR OR NOT MYSQL_LIBRARY OR NOT MYSQL_LIBRARY_PATH)
  include(mysqldb)
endif()

if (NOT MYSQL_FIND_QUIETLY)
  message(STATUS "MySQL Library: ${MYSQL_LIBRARY}")
  message(STATUS "MySQL Library Path: ${MYSQL_LIBRARY_PATH}")
  message(STATUS "MySQL Include Path: ${MYSQL_INCLUDE_DIR}")
  message(STATUS "MySQL Version: ${MYSQL_VERSION_STRING}")
endif ()

# handle the QUIETLY and REQUIRED arguments and set MySQL_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MySQL
    REQUIRED_VARS MYSQL_LIBRARY MYSQL_LIBRARY_PATH MYSQL_INCLUDE_DIR
    VERSION_VAR MYSQL_VERSION_STRING
    )

set(MYSQL_INCLUDE_DIRS ${MYSQL_INCLUDE_DIR})
set(MYSQL_LIBRARIES ${MYSQL_LIBRARY})

if (MySQL_FOUND AND NOT TARGET mysql::mysql)
  add_library(mysql::mysql UNKNOWN IMPORTED)
  set_target_properties(mysql::mysql PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${MYSQL_INCLUDE_DIR}")
  set_property(TARGET mysql::mysql APPEND PROPERTY IMPORTED_LOCATION "${MYSQL_LIBRARY}")
endif ()

mark_as_advanced(MYSQL_INCLUDE_DIR MYSQL_LIBRARY MYSQL_LIBRARY_PATH MYSQL_VERSION_STRING)

