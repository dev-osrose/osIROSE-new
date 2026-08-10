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

# On Windows there is no system sqlite3, so Build-sqlite3 compiles the
# amalgamation into a target of our own and aliases it as SQLite3::SQLite3.
# sqlpp23 links that alias into sqlpp23_sqlite3 and hands the target to
# install(EXPORT Sqlpp23Targets); CMake then refuses to generate the export
# because our SQLite3 target belongs to no export set. Nothing installs sqlpp23
# (see EXCLUDE_FROM_ALL above), so scope the dependency to the build tree with
# $<BUILD_LOCAL_INTERFACE:...>: targets in this build still link sqlite3, and
# the export drops the mention. Unix keeps the plain link - there SQLite3 is an
# imported target, which exports without complaint.
if(TARGET SQLite3)
  get_target_property(_sqlite3_is_imported SQLite3 IMPORTED)
  if(NOT _sqlite3_is_imported)
    foreach(_sqlpp23_target sqlpp23_sqlite3 sqlpp23_sqlite3_module)
      if(NOT TARGET ${_sqlpp23_target})
        continue()
      endif()

      get_target_property(_deps ${_sqlpp23_target} INTERFACE_LINK_LIBRARIES)
      set(_local_deps "")
      foreach(_dep IN LISTS _deps)
        if(_dep MATCHES "^SQLite3?::SQLite3$")
          set(_dep "$<BUILD_LOCAL_INTERFACE:${_dep}>")
        endif()
        list(APPEND _local_deps "${_dep}")
      endforeach()

      set_target_properties(${_sqlpp23_target} PROPERTIES
        INTERFACE_LINK_LIBRARIES "${_local_deps}")
    endforeach()

    unset(_deps)
    unset(_dep)
    unset(_local_deps)
    unset(_sqlpp23_target)
  endif()
  unset(_sqlite3_is_imported)
endif()
