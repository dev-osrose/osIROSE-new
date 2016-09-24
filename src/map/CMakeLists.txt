cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

LINK_DIRECTORIES(${EXTERNAL_BINARY_DIR})

SET(PROJECT MapServer)
project(${PROJECT})

IF(ENABLE_MYSQL_ONE_INSTANCE)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_MYSQL_ONE_INSTANCE")
ENDIF()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE")
set(GCC_LINK_FLAGS "${GCC_LINK_FLAGS}")
set(Clang_LINK_FLAGS "")
set(MSVC_LINK_FLAGS " /NODEFAULTLIB:MSVCRTD /NODEFAULTLIB:MSVCRT")

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
ELSE()
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -Werror")
ENDIF()

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-packed-bitfield-compat -Wl,-Map=${PROJECT}.map")
ELSEIF( ${CMAKE_CXX_COMPILER_ID} STREQUAL Clang )
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-keyword-compat")
ENDIF()

# Our Stuff 
include_directories(
	${CMAKE_SOURCE_DIR}/src/core/include
	${CMAKE_SOURCE_DIR}/src/proto/include
	${CMAKE_SOURCE_DIR}/src/rosecommon/include
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

SET( SOURCES_HEADER
	include/cmapserver
	include/cmapclient
	include/cmapisc )

SET( SOURCES_MAIN
	src/main.cpp
	src/cmapserver
	src/cmapclient
	src/cmapisc )

find_package( Threads REQUIRED )
add_executable(${PROJECT} ${SOURCES_MAIN} ${SOURCES_HEADER})

#IF (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
ADD_DEPENDENCIES(${PROJECT} protoGen)

IF(BUILD_LUA)
  ADD_DEPENDENCIES(${PROJECT} lua)
ENDIF()

IF(BUILD_MYSQLPP) 
  ADD_DEPENDENCIES(${PROJECT} mysqlpp)
ENDIF()

IF(BUILD_PROTOBUF)
  ADD_DEPENDENCIES(${PROJECT} libprotobuf)
ENDIF()
#ENDIF()

SET_TARGET_PROPERTIES(${PROJECT} PROPERTIES LINK_FLAGS
                "${${CMAKE_CXX_COMPILER_ID}_LINK_FLAGS}")
				
IF(ENABLE_MYSQL)
	TARGET_LINK_LIBRARIES(${PROJECT}
			${MYSQL_LIBRARIES}
        )
ENDIF()

target_link_libraries(${PROJECT} 
	Core 
	RoseCommon
	protoGen
	lua
	mysqlpp
	${CMAKE_THREAD_LIBS_INIT}
)

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
        TARGET_LINK_LIBRARIES(${PROJECT}
                wsock32.lib
                libprotobuf
        )
ELSE()
        TARGET_LINK_LIBRARIES(${PROJECT}
                mysqlclient
                protobuf
                )
ENDIF()

INSTALL(
    TARGETS ${PROJECT}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
    CONFIGURATIONS Release)
    
