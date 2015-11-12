cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project("Char Server")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(std "-std=c++11")
	set(warnings "-Wall -Wextra -Werror -Wno-write-strings")
	set(errors "")
	set(defines "")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(std "")
	set(warnings "/W4 /WX /EHsc")
	set(errors "")
	set(defines "")
endif()

if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "${std} ${defines} ${warnings} ${errors}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

# 3rd Party Libaries
include_directories(BEFORE SYSTEM $ENV{BOOST_ROOT} 
			${CMAKE_HOME_DIRECTORY}/3rdParty/include)
link_directories(${CMAKE_HOME_DIRECTORY}/3rdParty/lib)


# Our Stuff 
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(CharServer main.cpp extrafunction.cpp signal.cpp community.cpp clan.cpp charserver.cpp charpackets.cpp charclient.cpp)
target_link_libraries(CharServer Common mysqlcppconn-static mysqlclient)

set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")