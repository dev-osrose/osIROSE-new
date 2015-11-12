cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project("World Server")

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

list( fileHandlers
		./aip/AiActions.cpp
		./aip/AiConditions.cpp
		./aip/Aip.cpp
		./quest/QuestActions.cpp
		./quest/QuestConditions.cpp
		./quest/Quests.cpp )

list( mainSource 
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
target_link_libraries(CharServer Common mysqlcppconn-static mysqlclient)

set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")