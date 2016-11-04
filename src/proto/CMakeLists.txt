cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

SET(PROJECT protoGen)
project(${PROJECT})

IF (MSVC)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
ELSE()
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ENDIF()

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
ELSEIF( ${CMAKE_CXX_COMPILER_ID} STREQUAL Clang )
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
ENDIF()

include_directories(
	${PROTOBUF_INCLUDE_DIRS}
)

SET( ProtoFiles
	configfile.proto
	iscpackets.proto
)

SET(args 
	  PROTOFILES ${ProtoFiles}
	  OUTPATH ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

IF (DEBUG_PROTOBUF)
  MESSAGE("${args}")
  SET(args 
	  ${args}
	  DEBUG
  )
ENDIF()

PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${args})
ADD_LIBRARY(${PROJECT} STATIC ${ProtoSources} ${ProtoHeaders})
target_include_directories(${PROJECT} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

IF (BUILD_PROTOBUF)
  IF (MSVC)
    ADD_DEPENDENCIES(${PROJECT} protoc)
  ELSE()
    target_link_libraries(${PROJECT} PRIVATE libprotobuf)
    #ADD_DEPENDENCIES(${PROJECT} libprotobuf)
  ENDIF()
ENDIF()

