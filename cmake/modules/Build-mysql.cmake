set(MYSQL_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

ExternalProject_Add(
  mysql
  URL https://dev.mysql.com/get/Downloads/MySQL-8.0/mysql-boost-8.0.12.tar.gz
  URL_HASH MD5=82ba7cc22755f77635768c6e6d184161
  #CMAKE_ARGS -G${CMAKE_GENERATOR} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${MYSQL_INSTALL_DIR} -DWITHOUT_SERVER=ON -DWITH_UNIT_TESTS=OFF -DWITH_BOOST=<SOURCE_DIR>/boost/boost_1_67_0
  CMAKE_ARGS -G${CMAKE_GENERATOR} -DCMAKE_INSTALL_PREFIX=${MYSQL_INSTALL_DIR} -DWITHOUT_SERVER=ON -DWITH_UNIT_TESTS=OFF -DWITH_BOOST=<SOURCE_DIR>/boost/boost_1_67_0
  INSTALL_DIR ${MYSQL_INSTALL_DIR}
)
ExternalProject_Get_Property(
  mysql
  install_dir
)

set(MYSQL_INCLUDE_DIR "${install_dir}/include")
if(WIN32)
  set(MYSQL_LIBRARIES "${install_dir}/lib/libmysqlclient.lib" "${MYSQL_LIBRARIES}")
else()
  set(MYSQL_LIBRARIES "${install_dir}/lib/libmysqlclient.so" "${MYSQL_LIBRARIES}")
endif()
if(NOT TARGET mysql::mysql)
  add_library(mysql::mysql INTERFACE IMPORTED)
  set_target_properties(mysql::mysql PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${MYSQL_INCLUDE_DIR}")
  set_target_properties(mysql::mysql PROPERTIES INTERFACE_LINK_LIBRARIES "${MYSQL_LIBRARIES}")
endif()