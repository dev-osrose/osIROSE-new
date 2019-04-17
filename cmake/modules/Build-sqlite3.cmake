set(SQLITE3_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

message(STATUS "Building sqlite3 since sqlite3 was not found")

set(_byproducts
  ${SQLITE3_INSTALL_DIR}/lib/libsqlite3.lib
  ${SQLITE3_INSTALL_DIR}/lib/libsqlite3.so
  ${SQLITE3_INSTALL_DIR}/lib/libsqlite3.a
)

if(WIN32 OR MINGW)
  if(MINGW)
    ExternalProject_Add(
      sqlite3
      URL https://sqlite.org/2019/sqlite-autoconf-3270200.tar.gz
      URL_HASH SHA1=5f5750e3f39b7b60394a2fb6ddb2371f848670e6
      CONFIGURE_COMMAND ""
      BUILD_COMMAND gcc -shared sqlite3.c -o ${SQLITE3_INSTALL_DIR}/lib/libsqlite3.a
      BUILD_IN_SOURCE true
      BUILD_BYPRODUCTS ${_byproducts}
      INSTALL_COMMAND ${CMAKE_SCRIPT_PATH}/robocopy.bat "<SOURCE_DIR>/src" "<INSTALL_DIR>/include/" "*.h"
      INSTALL_DIR ${SQLITE3_INSTALL_DIR}
    )
  else()
    ExternalProject_Add(
      sqlite3
      URL https://sqlite.org/2019/sqlite-amalgamation-3270200.zip
      URL_HASH SHA1=0bafa4f52c19b8a917ad31048d22787d2e25be68
      CONFIGURE_COMMAND ""
      BUILD_COMMAND cl /c /EHsc sqlite3.c && lib sqlite3.obj /OUT:${SQLITE3_INSTALL_DIR}/lib/libsqlite3.lib
      BUILD_IN_SOURCE true
      BUILD_BYPRODUCTS ${_byproducts}
      INSTALL_COMMAND ${CMAKE_SCRIPT_PATH}/robocopy.bat "<SOURCE_DIR>" "<INSTALL_DIR>/include/" "*.h"
      INSTALL_DIR ${SQLITE3_INSTALL_DIR}
    )
  endif()
else()
  ExternalProject_Add(
    sqlite3
    URL https://sqlite.org/2019/sqlite-autoconf-3270200.tar.gz
    URL_HASH SHA1=5f5750e3f39b7b60394a2fb6ddb2371f848670e6
    CONFIGURE_COMMAND ./configure --prefix ${SQLITE3_INSTALL_DIR}
    BUILD_COMMAND make
    BUILD_IN_SOURCE true
    BUILD_BYPRODUCTS ${_byproducts}
    INSTALL_COMMAND make install
    INSTALL_DIR ${SQLITE3_INSTALL_DIR}
  )
endif()


ExternalProject_Get_Property(
  sqlite3
  install_dir
)

set(SQLITE3_INCLUDE_DIR "${install_dir}/include")
if(NOT MINGW AND WIN32)
  set(SQLITE3_LIBRARY_DIR "${install_dir}/lib")
  set(SQLITE3_LIBRARY "${install_dir}/lib/libsqlite3.lib")
  set(SQLITE3_LIBRARIES "${install_dir}/lib/libsqlite3.lib" "${SQLITE3_LIBRARIES}")
else()
  set(SQLITE3_LIBRARY_DIR "${install_dir}/lib")
  set(SQLITE3_LIBRARY "${install_dir}/lib/libsqlite3.a")
  set(SQLITE3_LIBRARIES "${install_dir}/lib/libsqlite3.a" "${SQLITE3_LIBRARIES}")
endif()
if(NOT TARGET sqlite3::sqlite3)
  add_library(sqlite3::sqlite3 INTERFACE IMPORTED)
  add_dependencies(sqlite3::sqlite3 sqlite3)
  set_target_properties(sqlite3::sqlite3 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${SQLITE3_INCLUDE_DIR}")
  set_target_properties(sqlite3::sqlite3 PROPERTIES INTERFACE_LINK_LIBRARIES "${SQLITE3_LIBRARIES}")
endif()

mark_as_advanced( SQLITE3_INCLUDE_DIR SQLITE3_LIBRARIES SQLITE3_LIBRARY SQLITE3_LIBRARY_PATH )