cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("Common")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(BEFORE SYSTEM $ENV{BOOST_ROOT})
include_directories(SYSTEM ${CMAKE_HOME_DIRECTORY}/3rdParty/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
link_directories(${CMAKE_HOME_DIRECTORY}/3rdParty/lib)

list( APPEND COMMON_LIST_SRC
  ./src/crosecrypt.cpp )

list( APPEND OTHER_SRC
  ./src/ini_parser.cpp )
  
set(COMMON_SRCS_TMP "")
foreach (COMMON_SRC ${COMMON_LIST_SRC})
	if (NOT "${COMMON_SRC}" MATCHES "^/")
		set(COMMON_SRC ${CMAKE_CURRENT_SOURCE_DIR}/${COMMON_SRC})
	endif()
	list(APPEND COMMON_SRCS_TMP ${COMMON_SRC})
endforeach()
  
set(COV_SRC_LIST "${COMMON_SRCS_TMP}" CACHE INTERNAL "")

add_library(Common STATIC ${COMMON_LIST_SRC} ${OTHER_SRC})
