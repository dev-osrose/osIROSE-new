cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("Login Server")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(std "")
	set(warnings "")
	set(errors "")
	set(defines "")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(std "")
	set(warnings "")
	set(errors "")
	set(defines "")
endif()

if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "$(CMAKE_CXX_FLAGS} ${std} ${defines} ${warnings} ${errors}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

# 3rd Party Libaries
include_directories(BEFORE SYSTEM $ENV{BOOST_ROOT} 
			${CMAKE_HOME_DIRECTORY}/3rdParty/include)
link_directories(${CMAKE_HOME_DIRECTORY}/3rdParty/lib)


# Our Stuff 
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

list( APPEND LOGIN_LIST_SRC
  main.cpp 
loginserver.cpp 
extrafunction.cpp 
loginclient.cpp 
loginpackets.cpp 
signal.cpp )

set(COMMON_SRCS_TMP "")
foreach (COMMON_SRC ${LOGIN_LIST_SRC})
	if (NOT "${COMMON_SRC}" MATCHES "^/")
		set(COMMON_SRC ${CMAKE_CURRENT_SOURCE_DIR}/${COMMON_SRC})
	endif()
	list(APPEND COMMON_SRCS_TMP ${COMMON_SRC})
endforeach()

set(COV_SRC_LIST "${COV_SRC_LIST};${COMMON_SRCS_TMP}" CACHE INTERNAL "")

find_package( Threads REQUIRED )
add_executable(LoginServer ${LOGIN_LIST_SRC})
target_link_libraries(LoginServer Common mysqlcppconn-static mysqlclient ${CMAKE_THREAD_LIBS_INIT})

set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")
