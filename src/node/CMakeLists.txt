cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT NodeServer)
project(${PROJECT})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE")
set(GCC_LINK_FLAGS "${GCC_LINK_FLAGS}")
set(Clang_LINK_FLAGS "")
set(MSVC_LINK_FLAGS " /NODEFAULTLIB:MSVCRTD /NODEFAULTLIB:MSVCRT")

IF (NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -Werror")
ENDIF()

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-packed-bitfield-compat -Wl,-Map=${PROJECT}.map")
ELSEIF( ${CMAKE_CXX_COMPILER_ID} STREQUAL Clang )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-keyword-compat")
ENDIF()

SET( SOURCES_HEADER
	include/cnodeserver
	include/cnodeclient
)

SET( SOURCES_MAIN
	src/main.cpp
	src/cnodeserver
	src/cnodeclient
)

find_package( Threads REQUIRED )
add_executable(${PROJECT} ${SOURCES_MAIN} ${SOURCES_HEADER})

target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
set_target_properties(${PROJECT} PROPERTIES LINK_FLAGS "${${CMAKE_CXX_COMPILER_ID}_LINK_FLAGS}")

IF (MSVC)
  target_link_libraries(${PROJECT} wsock32.lib)
  target_link_libraries(${PROJECT} libprotobuf)
ELSE()
  target_link_libraries(${PROJECT} ${PROTOBUF_LIBRARY})
ENDIF()
