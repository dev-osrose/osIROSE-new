cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

link_directories(${EXTERNAL_BINARY_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(PROJECT RoseCommon)
project(${PROJECT})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE")

IF (NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -Werror")
ENDIF()

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-packed-bitfield-compat")
ELSEIF( ${CMAKE_CXX_COMPILER_ID} STREQUAL Clang )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-keyword-compat")
ENDIF()

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include/packets
)

set( SOURCES_HEADER
  include/crosesocket
  include/crosecrypt
  include/croseserver
  include/croseclient 
  include/croseisc
  include/crosepacket
  include/epackettype
  include/packetclasses
  include/charpackets
  include/mappackets
  include/packets
  include/item
  include/partybase
  include/components/advancedinfo
  include/components/basicinfo
  include/components/bulletitems
  include/components/charactergraphics
  include/components/characterinfo
  include/components/destination
  include/components/graphics
  include/components/hotbar
  include/components/inventory
  include/components/position
  include/components/ridingitems
  include/components/skills
  include/components/socketconnector
  include/components/stats
  include/components/statuseffects
)

SET(SOURCES_BASE
  src/crosesocket
  src/crosecrypt 
  src/croseserver
  src/croseclient
  src/croseisc
  src/charpackets
  src/mappackets
  src/item
  src/entitycomponents
)

SET(SOURCES_PACKETS
  src/packets/cli_acceptreq 
  include/packets/cli_acceptreq
  src/packets/cli_alive 
  include/packets/cli_alive
  src/packets/cli_changemapreq 
  include/packets/cli_changemapreq
  src/packets/cli_channellistreq 
  include/packets/cli_channellistreq
  src/packets/cli_createcharreq 
  include/packets/cli_createcharreq
  src/packets/cli_deletecharreq 
  include/packets/cli_deletecharreq
  src/packets/cli_joinserverreq 
  include/packets/cli_joinserverreq
  src/packets/cli_loginreq 
  include/packets/cli_loginreq
  src/packets/cli_logoutreq 
  include/packets/cli_logoutreq
  src/packets/cli_mousecmd 
  include/packets/cli_mousecmd
  src/packets/cli_normalchat 
  include/packets/cli_normalchat
  src/packets/cli_revivereq 
  include/packets/cli_revivereq
  src/packets/cli_screenshottimereq 
  include/packets/cli_screenshottimereq
  src/packets/cli_selectcharreq 
  include/packets/cli_selectcharreq
  src/packets/cli_srvselectreq 
  include/packets/cli_srvselectreq
  src/packets/cli_stop 
  include/packets/cli_stop
  src/packets/cli_stopmoving 
  include/packets/cli_stopmoving
  src/packets/cli_whisperchat 
  include/packets/cli_whisperchat
  src/packets/cli_equipitem
  include/packets/cli_equipitem
  src/packets/cli_partyreq
  include/packets/cli_partyreq
  src/packets/cli_partyreply
  include/packets/cli_partyreply
  src/packets/cli_partychat
  include/packets/cli_partychat
  #------------------------------------------------
  src/packets/srv_acceptreply 
  include/packets/srv_acceptreply
#  src/packets/srv_changemap 
  include/packets/srv_changemap
  src/packets/srv_createcharreply 
  include/packets/srv_createcharreply
  src/packets/srv_deletecharreply 
  include/packets/srv_deletecharreply
  src/packets/srv_initdata 
  include/packets/srv_initdata
  src/packets/srv_inventorydata 
  include/packets/srv_inventorydata
  src/packets/srv_joinserverreply 
  include/packets/srv_joinserverreply
  src/packets/srv_logoutreply 
  include/packets/srv_logoutreply
  src/packets/srv_mousecmd 
  include/packets/srv_mousecmd
  src/packets/srv_normalchat 
  include/packets/srv_normalchat
  src/packets/srv_playerchar 
  include/packets/srv_playerchar
  src/packets/srv_removeobject 
  include/packets/srv_removeobject
  src/packets/srv_revivereply 
  include/packets/srv_revivereply
  src/packets/srv_screenshottimereply 
  include/packets/srv_screenshottimereply
  src/packets/srv_selectcharreply 
  include/packets/srv_selectcharreply
#  src/packets/srv_serverdata 
  include/packets/srv_serverdata
  src/packets/srv_srvselectreply 
  include/packets/srv_srvselectreply
  src/packets/srv_stop 
  include/packets/srv_stop
  src/packets/srv_stopmoving 
  include/packets/srv_stopmoving
  src/packets/srv_switchserver 
  include/packets/srv_switchserver
  src/packets/srv_whisperchat 
  include/packets/srv_whisperchat
  src/packets/srv_equipitem
  include/packets/srv_equipitem
  src/packets/srv_setmoney
  include/packets/srv_setmoney
  src/packets/srv_setitem
  include/packets/srv_setitem
  src/packets/srv_partyreq
  include/packets/srv_partyreq
  src/packets/srv_partyreply
  include/packets/srv_partyreply
  src/packets/srv_partymember
  include/packets/srv_partymember
  src/packets/srv_partychat
  include/packets/srv_partychat
  src/packets/srv_loginreply
  include/packets/srv_loginreply
  src/packets/srv_channellistreply
  include/packets/srv_channellistreply
)

#AUX_SOURCE_DIRECTORY(src/packets SOURCES_PACKETS) # CMAKE docs say to not use this function as CMAKE will not know when to reload the project files

add_library(${PROJECT} STATIC ${SOURCES_BASE} ${SOURCES_PACKETS} ${SOURCES_HEADER})
target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/packets>)
target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/components>)

target_link_libraries(${PROJECT} Core)
target_link_libraries(${PROJECT} lua)


IF (MSVC)
#  ADD_DEPENDENCIES(${PROJECT} libprotobuf)
#  ADD_DEPENDENCIES(${PROJECT} protoGen)
ENDIF()
