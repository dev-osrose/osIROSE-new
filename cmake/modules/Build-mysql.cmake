set(MYSQL_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

message(STATUS "Building MySQL since MySQL not found")

set(_byproducts
  ${MYSQL_INSTALL_DIR}/lib/libmysqlclient.lib
  ${MYSQL_INSTALL_DIR}/lib/libmysqlclient.so
  ${MYSQL_INSTALL_DIR}/lib/libmysqlclient.a
)

if(WIN32)
  ExternalProject_Add(
    mysql
    URL https://dev.mysql.com/get/Downloads/MySQL-8.0/mysql-8.0.17.zip
    URL_HASH MD5=12ffebf5a1a2b2047c6044b9b0e5860b
    CMAKE_ARGS -G${CMAKE_GENERATOR} -DCMAKE_INSTALL_PREFIX=${MYSQL_INSTALL_DIR} -DDOWNLOAD_BOOST=1 -DWITH_BOOST=<SOURCE_DIR>/boost -DWITHOUT_SERVER=ON -DWITH_UNIT_TESTS=OFF
    BUILD_BYPRODUCTS ${_byproducts}
    INSTALL_DIR ${MYSQL_INSTALL_DIR}
  )
else()
  ExternalProject_Add(
    mysql
    URL https://dev.mysql.com/get/Downloads/MySQL-8.0/mysql-boost-8.0.17.tar.gz
    URL_HASH MD5=7472a25d91973cbba5d8a8f176a3080b
    CMAKE_ARGS -G${CMAKE_GENERATOR} -DCMAKE_INSTALL_PREFIX=${MYSQL_INSTALL_DIR} -DWITHOUT_SERVER=ON -DWITH_UNIT_TESTS=OFF -DWITH_BOOST=<SOURCE_DIR>/boost/boost_1_69_0
    BUILD_BYPRODUCTS ${_byproducts}
    INSTALL_DIR ${MYSQL_INSTALL_DIR}
  )
endif()
ExternalProject_Get_Property(
  mysql
  install_dir
)

set(MYSQL_INCLUDE_DIR "${install_dir}/include")
if(WIN32)
  set(MYSQL_LIBRARY_DIR "${install_dir}/lib")
  set(MYSQL_LIBRARY "${install_dir}/lib/mysqlclient.lib")
  set(MYSQL_LIBRARIES "${install_dir}/lib/mysqlclient.lib" "${MYSQL_LIBRARIES}")
else()
  set(MYSQL_LIBRARY_DIR "${install_dir}/lib")
  set(MYSQL_LIBRARY "${install_dir}/lib/libmysqlclient.a")
  set(MYSQL_LIBRARIES "${install_dir}/lib/libmysqlclient.a" "${MYSQL_LIBRARIES}")
endif()
if(NOT TARGET mysql::mysql)
  add_library(mysql::mysql INTERFACE IMPORTED)
  add_dependencies(mysql::mysql mysql)
  set_target_properties(mysql::mysql PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${MYSQL_INCLUDE_DIR}")
  set_target_properties(mysql::mysql PROPERTIES INTERFACE_LINK_LIBRARIES "${MYSQL_LIBRARIES}")
endif()

mark_as_advanced( MYSQL_INCLUDE_DIR MYSQL_LIBRARIES MYSQL_LIBRARY )