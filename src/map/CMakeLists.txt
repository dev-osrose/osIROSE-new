
link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT MapServer)
project(${PROJECT})

#set(MSVC_LINK_FLAGS " /NODEFAULTLIB:MSVCRTD /NODEFAULTLIB:MSVCRT")
#add_linker_flags(/NODEFAULTLIB:MSVCRTD /NODEFAULTLIB:MSVCRT)

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  add_compiler_flags(-Wl,-Map=${PROJECT}.map)
ENDIF()

SET( SOURCES_HEADER
	include/cmapserver
	include/cmapclient
	include/cmapisc
  include/entitysystem
  include/systemmanager
  include/systems/system
  include/systems/movementsystem
  include/systems/timesystem
  include/systems/updatesystem
  include/systems/chatsystem
  include/systems/inventorysystem
  include/systems/partysystem
  include/systems/mapsystem
  )

SET( SOURCES_MAIN
	src/main.cpp
	src/cmapserver
	src/cmapclient
	src/cmapisc
  src/entitysystem
  src/systemmanager
  src/systems/movementsystem
  src/systems/timesystem
  src/systems/updatesystem
  src/systems/chatsystem
  src/systems/inventorysystem
  src/systems/partysystem
  src/systems/mapsystem
  )

add_executable(${PROJECT} ${SOURCES_MAIN} ${SOURCES_HEADER})

target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
set_target_properties(${PROJECT} PROPERTIES LINK_FLAGS "${${CMAKE_CXX_COMPILER_ID}_LINK_FLAGS}")
				
IF(WITH_MYSQL)
  target_link_libraries(${PROJECT} ${MYSQL_LIBRARIES})
ENDIF()

target_link_libraries(${PROJECT} RoseCommon)
target_link_libraries(${PROJECT} ${CMAKE_THREAD_LIBS_INIT})

IF (MSVC)
  target_link_libraries(${PROJECT} wsock32.lib)
ELSE()
    IF (WITH_MYSQL)
      target_link_libraries(${PROJECT} mysqlclient)
    ENDIF()
ENDIF()

generate_symbol_data(${PROJECT})
