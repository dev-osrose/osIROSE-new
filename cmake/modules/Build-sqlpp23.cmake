set(SQLPP23_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

# sqlpp23 has no option to turn its own install rules off, and we only consume
# its headers and connector libs. EXCLUDE_FROM_ALL suppresses the whole
# subdirectory's install rules, so `cmake --install` ships our servers without
# sqlpp23's headers, ddl2cpp script and cmake package config coming along.
#
# Do NOT reach for CMAKE_SKIP_INSTALL_RULES here: it is directory scoped, but
# the parent's cmake_install.cmake still unconditionally include()s the
# subdirectory's, which then never gets generated - `cmake --install` dies with
# "include could not find requested file".
FetchContent_Declare(
  sqlpp23
  GIT_REPOSITORY https://github.com/rbock/sqlpp23
  GIT_TAG 0.70
  EXCLUDE_FROM_ALL
)

# sqlpp23 settings
set(BUILD_MYSQL_CONNECTOR OFF)
set(BUILD_MARIADB_CONNECTOR ON)
set(BUILD_POSTGRESQL_CONNECTOR OFF)
set(BUILD_SQLITE3_CONNECTOR ON)
set(BUILD_SQLCIPHER_CONNECTOR OFF)

# sqlpp23 gates its own test tree on `PROJECT_IS_TOP_LEVEL AND BUILD_TESTING`,
# and FetchContent never makes it top level, so its tests stay off without us
# touching BUILD_TESTING. Forcing that cache variable here would clobber the
# one CTest sets for our own suite.
FetchContent_MakeAvailable(sqlpp23)
