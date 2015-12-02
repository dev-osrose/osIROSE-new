cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

INCLUDE_DIRECTORIES(
    SYSTEM ${PROTOBUF_INCLUDE_DIR}
	SYSTEM ${ASIO_INCLUDE_DIR}
	SYSTEM ${EXTERNAL_INCLUDE_DIR}
)

LINK_DIRECTORIES(${EXTERNAL_BINARY_DIR})

project("LoginServer")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE")

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
        IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
                SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MTd")
        ELSE()
                SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MT")
        ENDIF()
ELSE()
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Werror")
ENDIF()

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-packed-bitfield-compat")
ELSEIF( ${CMAKE_CXX_COMPILER_ID} STREQUAL Clang )
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-keyword-compat")
ENDIF()

# Our Stuff 
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

SET( SOURCES_MAIN
  src/main.cpp )

find_package( Threads REQUIRED )
add_executable(LoginServer ${SOURCES_MAIN})
target_link_libraries(LoginServer Core ${CMAKE_THREAD_LIBS_INIT})

INSTALL(
    TARGETS LoginServer
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
    CONFIGURATIONS Release)
