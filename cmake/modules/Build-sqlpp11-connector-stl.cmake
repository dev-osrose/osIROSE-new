set(SQLPP11_STL_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

if(WIN32)
  set(_byproducts
  )
else()
  set(_byproducts
  )
endif()

ExternalProject_Add(
  sqlpp11-connector-stl
  GIT_REPOSITORY https://github.com/rbock/sqlpp11-connector-stl
  GIT_TAG db2af44dd507a97fb85aa9cf84e1775f7f07086b
  CMAKE_ARGS -G ${CMAKE_GENERATOR} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${SQLPP11_STL_INSTALL_DIR} -DCMAKE_MODULE_PATH=${CMAKE_MODULE_PATH} -DSQLPP11_INCLUDE_DIR=${SQLPP11_INCLUDE_DIR}
  BUILD_BYPRODUCTS ${_byproducts}
  DEPENDS SQLPP11::SQLPP11
  INSTALL_DIR ${SQLPP11_STL_INSTALL_DIR}
)

ExternalProject_Get_Property(
  sqlpp11-connector-stl
  install_dir
)

set(SQLPP_CONNECTOR_STL_INCLUDE_DIR "${install_dir}/include")
if(WIN32)
  set(SQLPP11_STL_LIBRARIES "${install_dir}/lib/sqlpp-mysql.lib" "${MYSQL_LIBRARIES}")
else()
  set(SQLPP11_STL_LIBRARIES "${install_dir}/lib/libsqlpp-mysql.a" "${MYSQL_LIBRARIES}")
endif()
if(NOT TARGET SQLPP11::sqlpp11_stl)
    add_library(SQLPP11::sqlpp11_stl INTERFACE IMPORTED)
    add_dependencies(SQLPP11::sqlpp11_stl sqlpp11-connector-stl)
    set_target_properties(SQLPP11::sqlpp11_stl PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${SQLPP_CONNECTOR_STL_INCLUDE_DIR}")
    set_target_properties(SQLPP11::sqlpp11_stl PROPERTIES INTERFACE_LINK_LIBRARIES "${SQLPP11_STL_LIBRARIES}")
endif()