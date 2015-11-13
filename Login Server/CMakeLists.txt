cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
#set(CMAKE_LEGACY_CYGWIN_WIN32 0)

project("Login Server")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(std "-std=c++11")
	set(warnings "-Wall -Wextra -Werror -Wno-write-strings")
	set(errors "")
	set(defines "")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(std "")
	set(warnings "/EHsc")
	set(errors "")
	set(defines "")
	
	file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/LoginServer.vcxproj.user" "<?xml version=\"1.0\" encoding=\"utf-8\"?> 
<Project xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\"> 
	<Import Project=\"${CMAKE_HOME_DIRECTORY}/AZGStudioProjectSettings.props\" /> 
</Project>" )
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

add_executable(LoginServer main.cpp loginserver.cpp extrafunction.cpp loginclient.cpp loginpackets.cpp signal.cpp)
target_link_libraries(LoginServer Common mysqlcppconn-static mysqlclient)

set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")