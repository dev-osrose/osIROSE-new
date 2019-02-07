set(SQLPP11_SQLITE3_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

set(_byproducts
    ${SQLPP11_SQLITE3_INSTALL_DIR}/lib/libsqlpp11-connector-sqlite3.lib
    ${SQLPP11_SQLITE3_INSTALL_DIR}/lib/libsqlpp11-connector-sqlite3.a
  )

ExternalProject_Add(
  sqlpp11-connector-sqlite3
  GIT_REPOSITORY https://github.com/rbock/sqlpp11-connector-sqlite3
  GIT_TAG bde9fb214f36fee22e9d2f81dce95759cf4de287
  CMAKE_ARGS -G ${CMAKE_GENERATOR} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${SQLPP11_SQLITE3_INSTALL_DIR} -DCMAKE_MODULE_PATH=${CMAKE_MODULE_PATH} -DSQLPP11_INCLUDE_DIR=${SQLPP11_INCLUDE_DIR} -DDATE_INCLUDE_DIR=${DATE_INCLUDE_DIR} -DENABLE_TESTS=OFF
  BUILD_BYPRODUCTS ${_byproducts}
  DEPENDS SQLPP11::SQLPP11
  DEPENDS sqlite3::sqlite3
  INSTALL_DIR ${SQLPP11_SQLITE3_INSTALL_DIR}
)

ExternalProject_Get_Property(
  sqlpp11-connector-sqlite3
  install_dir
)

set(SQLPP_CONNECTOR_SQLITE3_INCLUDE_DIR "${install_dir}/include")
if(NOT MINGW AND WIN32)
  set(SQLPP11_SQLITE3_LIBRARIES "${install_dir}/lib/sqlpp11-connector-sqlite3.lib" "${SQLITE3_LIBRARIES}")
else()
  set(SQLPP11_SQLITE3_LIBRARIES "${install_dir}/lib/libsqlpp11-connector-sqlite3.a" "${SQLITE3_LIBRARIES}")
endif()
if(NOT TARGET SQLPP11::sqlpp11_sqlite3)
    add_library(SQLPP11::sqlpp11_sqlite3 INTERFACE IMPORTED)
    add_dependencies(SQLPP11::sqlpp11_sqlite3 sqlpp11-connector-sqlite3)
    set_target_properties(SQLPP11::sqlpp11_sqlite3 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${SQLPP_CONNECTOR_SQLITE3_INCLUDE_DIR}")
    set_target_properties(SQLPP11::sqlpp11_sqlite3 PROPERTIES INTERFACE_LINK_LIBRARIES "${SQLPP11_SQLITE3_LIBRARIES}")
endif()