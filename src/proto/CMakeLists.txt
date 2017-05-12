cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(PROJECT protoGen)
project(${PROJECT})

IF (MSVC)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
ELSE()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ENDIF()

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
ELSEIF( ${CMAKE_CXX_COMPILER_ID} STREQUAL Clang )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
ENDIF()

include_directories(
	${PROTOBUF_INCLUDE_DIRS}
)

set( ProtoFiles
  configfile.proto
  iscpackets.proto
)

set(args
  PROTOFILES ${ProtoFiles}
  OUTPATH ${CMAKE_BINARY_DIR}/generated
)

IF (DEBUG_PROTOBUF)
  message("${args}")
  set(args
    ${args}
    DEBUG
  )
ENDIF()

PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${args})
add_library(${PROJECT} STATIC ${ProtoSources} ${ProtoHeaders})
target_include_directories(${PROJECT} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/generated>)

IF (BUILD_PROTOBUF)
  IF (MSVC)
    add_dependencies(${PROJECT} protoc)
  ELSE()
    target_link_libraries(${PROJECT} libprotobuf)
    #add_dependencies(${PROJECT} libprotobuf)
  ENDIF()
ENDIF()

