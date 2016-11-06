cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

link_directories(${EXTERNAL_BINARY_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(PROJECT RoseCommon)
project(${PROJECT})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE")

IF (NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Werror")
ENDIF()

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-packed-bitfield-compat")
ELSEIF( ${CMAKE_CXX_COMPILER_ID} STREQUAL Clang )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-keyword-compat")
ENDIF()

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include/packets
)

set( SOURCES_HEADER
  include/crosecrypt
  include/croseserver
  include/croseclient 
  include/croseisc
  include/crosepacket
  include/epackettype
  include/findclass
  include/iscontainer
  include/packetclasses
  include/rosepackets
  include/loginpackets
  include/charpackets
  include/mappackets
)

SET( SOURCES_BASE
  src/crosecrypt 
  src/croseserver
  src/croseclient
  src/croseisc
  src/loginpackets
  src/charpackets
  src/mappackets
)

add_library(${PROJECT} STATIC ${SOURCES_BASE} ${SOURCES_HEADER})
target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/packets>)

target_link_libraries(${PROJECT} Core)
target_link_libraries(${PROJECT} lua)


IF (MSVC)
#  ADD_DEPENDENCIES(${PROJECT} libprotobuf)
#  ADD_DEPENDENCIES(${PROJECT} protoGen)
ENDIF()
