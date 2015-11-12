cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
#set(CMAKE_LEGACY_CYGWIN_WIN32 0)

project("Common")

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

include_directories(BEFORE SYSTEM $ENV{BOOST_ROOT})
include_directories(SYSTEM ${CMAKE_HOME_DIRECTORY}/3rdParty/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
link_directories(${CMAKE_HOME_DIRECTORY}/3rdParty/lib)

add_library(Common STATIC config.cpp globals.cpp log.cpp rosecrypt.cpp SocketBase.cpp SocketClient.cpp SocketServer.cpp ./database/database.cpp)

set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")
