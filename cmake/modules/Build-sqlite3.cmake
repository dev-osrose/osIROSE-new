# Only do this for windows platforms
if(WIN32 OR MINGW)
  set(SQLITE3_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})
  set(CMAKE_INSTALL_PREFIX ${CMAKE_THIRD_PARTY_DIR})
  set(INSTALL_PLUGINDIR ${CMAKE_INSTALL_PREFIX})

  message(STATUS "Building sqlite3 since sqlite3 was not found")

  set(_byproducts
      ${SQLITE3_INSTALL_DIR}/lib/libsqlite3.lib
      ${SQLITE3_INSTALL_DIR}/lib/libsqlite3.so
      ${SQLITE3_INSTALL_DIR}/lib/libsqlite3.a
  )

  set(_build_command "")
  if(MINGW)
    set(_build_command "gcc -shared sqlite3.c -o ${SQLITE3_INSTALL_DIR}/lib/libsqlite3.a")
  else()
    set(_build_command "cl /c /EHsc sqlite3.c && lib sqlite3.obj /OUT:${SQLITE3_INSTALL_DIR}/lib/libsqlite3.lib")
  endif()

  FetchContent_Declare(
      sqlite3
      URL https://www.sqlite.org/2023/sqlite-amalgamation-3430100.zip
      URL_HASH SHA3-256=2110f85a90033016f5b5691812448e8dfa606fe407f4b2ebfec66dda2968fe3c
      DOWNLOAD_EXTRACT_TIMESTAMP
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ${_build_command}
#      BUILD_IN_SOURCE true
      BUILD_BYPRODUCTS ${_byproducts}
      INSTALL_COMMAND ${CMAKE_SCRIPT_PATH}/robocopy.bat "<SOURCE_DIR>" "<INSTALL_DIR>/include/" "*.h"
      INSTALL_DIR ${SQLITE3_INSTALL_DIR}
  )

  FetchContent_MakeAvailable(sqlite3)
endif()

#if(WIN32 OR MINGW)
#  if(MINGW)
#    ExternalProject_Add(
#      sqlite3
#      URL https://sqlite.org/2019/sqlite-autoconf-3270200.tar.gz
#      URL_HASH SHA1=5f5750e3f39b7b60394a2fb6ddb2371f848670e6
#      DOWNLOAD_EXTRACT_TIMESTAMP TRUE
#      CONFIGURE_COMMAND ""
#      BUILD_COMMAND gcc -shared sqlite3.c -o ${SQLITE3_INSTALL_DIR}/lib/libsqlite3.a
#      BUILD_IN_SOURCE true
#      BUILD_BYPRODUCTS ${_byproducts}
#      INSTALL_COMMAND ${CMAKE_SCRIPT_PATH}/robocopy.bat "<SOURCE_DIR>/src" "<INSTALL_DIR>/include/" "*.h"
#      INSTALL_DIR ${SQLITE3_INSTALL_DIR}
#    )
#  else()
#    ExternalProject_Add(
#      sqlite3
#      URL https://sqlite.org/2019/sqlite-amalgamation-3270200.zip
#      URL_HASH SHA1=0bafa4f52c19b8a917ad31048d22787d2e25be68
#      DOWNLOAD_EXTRACT_TIMESTAMP TRUE
#      CONFIGURE_COMMAND ""
#      BUILD_COMMAND cl /c /EHsc sqlite3.c && lib sqlite3.obj /OUT:${SQLITE3_INSTALL_DIR}/lib/libsqlite3.lib
#      BUILD_IN_SOURCE true
#      BUILD_BYPRODUCTS ${_byproducts}
#      INSTALL_COMMAND ${CMAKE_SCRIPT_PATH}/robocopy.bat "<SOURCE_DIR>" "<INSTALL_DIR>/include/" "*.h"
#      INSTALL_DIR ${SQLITE3_INSTALL_DIR}
#    )
#  endif()
#else()
#  ExternalProject_Add(
#    sqlite3
#    URL https://sqlite.org/2019/sqlite-autoconf-3270200.tar.gz
#    URL_HASH SHA1=5f5750e3f39b7b60394a2fb6ddb2371f848670e6
#    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
#    CONFIGURE_COMMAND ./configure --prefix ${SQLITE3_INSTALL_DIR}
#    BUILD_COMMAND make
#    BUILD_IN_SOURCE true
#    BUILD_BYPRODUCTS ${_byproducts}
#    INSTALL_COMMAND make install
#    INSTALL_DIR ${SQLITE3_INSTALL_DIR}
#  )
#endif()
#
#
#ExternalProject_Get_Property(
#  sqlite3
#  install_dir
#)
#
#set(SQLite3_INCLUDE_DIR "${install_dir}/include" CACHE PATH "" FORCE)
#if(NOT MINGW AND WIN32)
#  set(SQLITE3_LIBRARY_DIR "${install_dir}/lib" CACHE PATH "" FORCE)
#  set(SQLITE3_LIBRARY_PATH "${SQLITE3_LIBRARY_DIR}" CACHE PATH "" FORCE)
#  set(SQLite3_LIBRARY "${install_dir}/lib/libsqlite3.lib" CACHE FILEPATH "" FORCE)
#  set(SQLITE3_LIBRARIES "${install_dir}/lib/libsqlite3.lib" "${SQLITE3_LIBRARIES}" CACHE STRING "" FORCE)
#else()
#  set(SQLITE3_LIBRARY_DIR "${install_dir}/lib" CACHE PATH "" FORCE)
#  set(SQLITE3_LIBRARY_PATH "${SQLITE3_LIBRARY_DIR}" CACHE PATH "" FORCE)
#  set(SQLite3_LIBRARY "${install_dir}/lib/libsqlite3.a" CACHE FILEPATH "" FORCE)
#  set(SQLITE3_LIBRARIES "${install_dir}/lib/libsqlite3.a" "dl" CACHE STRING "" FORCE)
#endif()
#if(NOT TARGET sqlite3::sqlite3)
#  add_library(sqlite3::sqlite3 INTERFACE IMPORTED)
#  add_dependencies(sqlite3::sqlite3 sqlite3)
#  set_target_properties(sqlite3::sqlite3 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${SQLite3_INCLUDE_DIR}")
#  set_target_properties(sqlite3::sqlite3 PROPERTIES INTERFACE_LINK_LIBRARIES "${SQLITE3_LIBRARIES}")
#endif()
#
#mark_as_advanced( SQLite3_INCLUDE_DIR SQLITE3_LIBRARIES SQLite3_LIBRARY SQLITE3_LIBRARY_PATH )