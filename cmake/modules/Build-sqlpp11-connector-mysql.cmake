set(SQLPP11_MYSQL_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

if(WIN32)
  set(_byproducts
    ${SQLPP11_MYSQL_INSTALL_DIR}/bin/sqlpp11-ddl2cpp.exe
    ${SQLPP11_MYSQL_INSTALL_DIR}/lib/libsqlpp-mysql.lib
  )
  ExternalProject_Add(
    sqlpp11-connector-mysql
    GIT_REPOSITORY https://github.com/RavenX8/sqlpp11-connector-mysql
    GIT_TAG c6e81fd9ffe8ff63d9d9fba84053aaca20194272
    CMAKE_ARGS -G ${CMAKE_GENERATOR} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DSQLPP11_INCLUDE_DIR=${SQLPP11_INCLUDE_DIR} -DDATE_INCLUDE_DIR=${DATE_INCLUDE_DIR} -DCMAKE_INSTALL_PREFIX=${SQLPP11_MYSQL_INSTALL_DIR} -DENABLE_TESTS=OFF
    BUILD_BYPRODUCTS ${_byproducts}
    DEPENDS SQLPP11::SQLPP11
    INSTALL_DIR ${SQLPP11_MYSQL_INSTALL_DIR}
  )
else()
  set(_byproducts
    ${SQLPP11_MYSQL_INSTALL_DIR}/bin/sqlpp11-ddl2cpp
    ${SQLPP11_MYSQL_INSTALL_DIR}/lib/libsqlpp-mysql.a
  )
  ExternalProject_Add(
    sqlpp11-connector-mysql
    GIT_REPOSITORY https://github.com/RavenX8/sqlpp11-connector-mysql
    GIT_TAG c6e81fd9ffe8ff63d9d9fba84053aaca20194272
    CMAKE_ARGS -G ${CMAKE_GENERATOR} -DCMAKE_INSTALL_PREFIX=${SQLPP11_MYSQL_INSTALL_DIR} -DSQLPP11_INCLUDE_DIR=${SQLPP11_INCLUDE_DIR} -DDATE_INCLUDE_DIR=${DATE_INCLUDE_DIR} -DENABLE_TESTS=OFF
    BUILD_BYPRODUCTS ${_byproducts}
    DEPENDS SQLPP11::SQLPP11
    INSTALL_DIR ${SQLPP11_MYSQL_INSTALL_DIR}
  )
endif()

ExternalProject_Get_Property(
  sqlpp11-connector-mysql
  install_dir
)

set(SQLPP_CONNECTOR_MYSQL_INCLUDE_DIR "${install_dir}/include")
if(WIN32)
  set(SQLPP11_MYSQL_LIBRARIES "${install_dir}/lib/libsqlpp-mysql.lib" "${MYSQL_LIBRARIES}")
  set(SQLPP11_EXE_PATH "${install_dir}/bin/sqlpp11-ddl2cpp.exe")
else()
  set(SQLPP11_MYSQL_LIBRARIES "${install_dir}/lib/libsqlpp-mysql.a" "${MYSQL_LIBRARIES}")
  set(SQLPP11_EXE_PATH "${install_dir}/bin/sqlpp11-ddl2cpp")
endif()
if(NOT TARGET SQLPP11::SQLPP11_MYSQL)
    add_library(SQLPP11::SQLPP11_MYSQL INTERFACE IMPORTED)
    add_dependencies(SQLPP11::SQLPP11_MYSQL sqlpp11-connector-mysql)
    set_target_properties(SQLPP11::SQLPP11_MYSQL PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${SQLPP_CONNECTOR_MYSQL_INCLUDE_DIR}")
    set_target_properties(SQLPP11::SQLPP11_MYSQL PROPERTIES INTERFACE_LINK_LIBRARIES "${SQLPP11_MYSQL_LIBRARIES}")
endif()