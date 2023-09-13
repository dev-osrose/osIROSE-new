set(MYSQL_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})
set(CMAKE_INSTALL_PREFIX ${CMAKE_THIRD_PARTY_DIR})
set(INSTALL_PLUGINDIR ${CMAKE_INSTALL_PREFIX})

message(STATUS "Building MySQL since MySQL not found")

#FetchContent_Declare(
#  mysql
#  URL https://dev.mysql.com/get/Downloads/MySQL-8.1/mysql-boost-8.1.0.tar.gz
#  URL_HASH MD5=43a081ea2a6bdc3dbc82c5478b9edda0
#  DOWNLOAD_EXTRACT_TIMESTAMP
#  SYSTEM
#  FIND_PACKAGE_ARGS NAMES MySQL
#)

FetchContent_Declare(
    mysql
    GIT_REPOSITORY https://github.com/MariaDB/server.git
    GIT_TAG mariadb-11.2.1
    SYSTEM
    FIND_PACKAGE_ARGS NAMES MySQL
)

# sqlpp11 settings
set(WITHOUT_SERVER ON)
set(WITH_UNIT_TESTS OFF)

FetchContent_MakeAvailable(mysql)
#FetchContent_GetProperties(mysql)
#if(NOT mysql_POPULATED)
#    # Fetch the content using previously declared details
#    FetchContent_Populate(mysql)
#
#    # Set custom variables, policies, etc.
#    # ...
#    set(WITH_BOOST ${mysql_SOURCE_DIR}/boost/boost_1_77_0)
#
#    # Bring the populated content into the build
#    add_subdirectory(${mysql_SOURCE_DIR} ${mysql_BINARY_DIR})
#endif()

#set(_byproducts
#  ${MYSQL_INSTALL_DIR}/lib/libmysqlclient.lib
#  ${MYSQL_INSTALL_DIR}/lib/libmysqlclient.so
#  ${MYSQL_INSTALL_DIR}/lib/libmysqlclient.a
#)
#
#if(WIN32)
#  ExternalProject_Add(
#    mysql
#    URL https://dev.mysql.com/get/Downloads/MySQL-8.1/mysql-8.1.0.zip
#    URL_HASH MD5=33eab7cbd4de99a3d9304fdec9955f0c
#    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
#    CMAKE_ARGS -G${CMAKE_GENERATOR} -DCMAKE_INSTALL_PREFIX=${MYSQL_INSTALL_DIR} -DDOWNLOAD_BOOST=1 -DWITH_BOOST=<SOURCE_DIR>/boost -DWITHOUT_SERVER=ON -DWITH_UNIT_TESTS=OFF
#    BUILD_BYPRODUCTS ${_byproducts}
#    INSTALL_DIR ${MYSQL_INSTALL_DIR}
#  )
#else()
#  ExternalProject_Add(
#    mysql
#    URL https://dev.mysql.com/get/Downloads/MySQL-8.1/mysql-boost-8.1.0.tar.gz
#    URL_HASH MD5=43a081ea2a6bdc3dbc82c5478b9edda0
#    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
#    CMAKE_ARGS -G${CMAKE_GENERATOR} -DCMAKE_INSTALL_PREFIX=${MYSQL_INSTALL_DIR} -DWITHOUT_SERVER=ON -DWITH_UNIT_TESTS=OFF -DWITH_BOOST=<SOURCE_DIR>/boost/boost_1_69_0
#    BUILD_BYPRODUCTS ${_byproducts}
#    INSTALL_DIR ${MYSQL_INSTALL_DIR}
#  )
#endif()
#ExternalProject_Get_Property(
#  mysql
#  install_dir
#)
#
#set(MYSQL_INCLUDE_DIR "${install_dir}/include" CACHE PATH "" FORCE)
#if(WIN32)
#  set(MYSQL_LIBRARY_DIR "${install_dir}/lib" CACHE PATH "" FORCE)
#  set(MYSQL_LIBRARY_PATH "${MYSQL_LIBRARY_DIR}" CACHE PATH "" FORCE)
#  set(MYSQL_LIBRARY "${install_dir}/lib/libmysqlclient.lib" CACHE FILEPATH "" FORCE)
#  set(MYSQL_LIBRARIES "${install_dir}/lib/libmysqlclient.lib" "${MYSQL_LIBRARIES}" CACHE STRING "" FORCE)
#else()
#  set(MYSQL_LIBRARY_DIR "${install_dir}/lib" CACHE PATH "" FORCE)
#  set(MYSQL_LIBRARY_PATH "${MYSQL_LIBRARY_DIR}" CACHE PATH "" FORCE)
#  set(MYSQL_LIBRARY "${install_dir}/lib/libmysqlclient.a" CACHE FILEPATH "" FORCE)
#  set(MYSQL_LIBRARIES "${install_dir}/lib/libmysqlclient.a" "${MYSQL_LIBRARIES}" CACHE STRING "" FORCE)
#endif()
#if(NOT TARGET MySQL::MySQL)
#  add_library(MySQL::MySQL INTERFACE IMPORTED)
#  add_dependencies(MySQL::MySQL mysql)
#  set_target_properties(MySQL::MySQL PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${MYSQL_INCLUDE_DIR}")
#  set_target_properties(MySQL::MySQL PROPERTIES INTERFACE_LINK_LIBRARIES "${MYSQL_LIBRARIES}")
#endif()
#
#mark_as_advanced( MYSQL_INCLUDE_DIR MYSQL_LIBRARY_PATH MYSQL_LIBRARIES MYSQL_LIBRARY )