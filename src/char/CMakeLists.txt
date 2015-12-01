cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

INCLUDE_DIRECTORIES(
	SYSTEM ${PROTOBUF_INCLUDE_DIR}
	SYSTEM ${ASIO_INCLUDE_DIR}
	SYSTEM ${EXTERNAL_INCLUDE_DIR}
)

LINK_DIRECTORIES(${EXTERNAL_BINARY_DIR})

project("CharServer")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic -Wextra -Werror")

# Our Stuff 
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

SET( SOURCES_MAIN
  src/main.cpp )

find_package( Threads REQUIRED )
add_executable(CharServer ${SOURCES_MAIN})
target_link_libraries(CharServer Core ${CMAKE_THREAD_LIBS_INIT})
