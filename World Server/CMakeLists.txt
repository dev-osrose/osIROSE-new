cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("World Server")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

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

list( APPEND fileHandlers
		./aip/AiActions.cpp
		./aip/AiConditions.cpp
		./aip/Aip.cpp
		./quest/QuestActions.cpp
		./quest/QuestConditions.cpp
		./quest/Quests.cpp )

list( APPEND mainSource 
	battle.cpp
	Buff.cpp
	character.cpp
	charevent.cpp
	charfunctions.cpp
	charstats.cpp
	clan.cpp
	extrafunctions.cpp
	gmcmds.cpp
	itemdata.cpp
	main.cpp
	MainProcess.cpp
	mapfunctions.cpp
	Monster.cpp
	MonsterEvents.cpp
	monsterfunctions.cpp
	MonsterStats.cpp
	Party.cpp
	partyfunctions.cpp
	player.cpp
	playerdata.cpp
	playerfunctions.cpp
	playerquest.cpp
	playerstats.cpp
	ServerFunctions.cpp
	startup.cpp
	stbhandler.cpp
	strhashing.cpp
	worldmap.cpp
	WorldMonster.cpp
	worldpackets.cpp
	Worldprocess.cpp
	worldserver.cpp )
	

add_executable(WorldServer ${mainSource} ${fileHandlers})
target_link_libraries(WorldServer Common mysqlcppconn-static mysqlclient)

set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")