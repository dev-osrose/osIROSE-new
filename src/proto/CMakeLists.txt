cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

SET(PROJECT protoGen)
project(${PROJECT})

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
#foreach(flag_var
#        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
#        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
#   if(${flag_var} MATCHES "/MD")
#      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
#   endif(${flag_var} MATCHES "/MD")
#endforeach(flag_var)
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

PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${args})
ADD_LIBRARY(${PROJECT} STATIC ${ProtoSources} ${ProtoHeaders})

IF (BUILD_PROTOBUF)
  ADD_DEPENDENCIES(${PROJECT} libprotobuf)
ENDIF()

