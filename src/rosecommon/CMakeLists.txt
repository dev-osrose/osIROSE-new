cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

LINK_DIRECTORIES(${EXTERNAL_BINARY_DIR})

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project("RoseCommon")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE")

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)

ELSE()
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Werror")
ENDIF()

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-packed-bitfield-compat")
ELSEIF( ${CMAKE_CXX_COMPILER_ID} STREQUAL Clang )
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-keyword-compat")
ENDIF()

INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}/src/core/include
	${CMAKE_SOURCE_DIR}/src/proto/include
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/include/packets
)

SET( SOURCES_HEADER
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

add_library(RoseCommon STATIC ${SOURCES_BASE} ${SOURCES_HEADER})

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
#	ADD_DEPENDENCIES(${PROJECT} libprotobuf)
	ADD_DEPENDENCIES(RoseCommon protoGen)
ENDIF()
