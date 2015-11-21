cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("LoginServer")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic -Wextra")

# 3rd Party Libaries
include_directories(BEFORE SYSTEM $ENV{BOOST_ROOT} 
			${CMAKE_HOME_DIRECTORY}/3rdParty/include)
link_directories(${CMAKE_HOME_DIRECTORY}/3rdParty/lib)


# Our Stuff 
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

list( APPEND LOGIN_LIST_SRC
  src/main.cpp )

find_package( Threads REQUIRED )
add_executable(LoginServer ${LOGIN_LIST_SRC})
target_link_libraries(LoginServer Common mysqlcppconn-static mysqlclient ${CMAKE_THREAD_LIBS_INIT})
