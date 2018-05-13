
link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT MapServer)
project(${PROJECT})

if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  add_compiler_flags(-Wl,-Map=${PROJECT}.map)
endif()

SET( SOURCES_HEADER
  include/cmapserver.h
  include/cmapclient.h
  include/cmapisc.h
  include/entitysystem.h
  include/systemmanager.h
  include/systems/system.h
  include/systems/movementsystem.h
  include/systems/timesystem.h
  include/systems/updatesystem.h
  include/systems/chatsystem.h
  include/systems/inventorysystem.h
  include/systems/partysystem.h
  include/systems/mapsystem.h
  )

SET( SOURCES_MAIN
  src/main
  src/cmapserver.cpp
  src/cmapclient.cpp
  src/cmapisc.cpp
  src/entitysystem.cpp
  src/systemmanager.cpp
  src/systems/movementsystem.cpp
  src/systems/timesystem.cpp
  src/systems/updatesystem.cpp
  src/systems/chatsystem.cpp
  src/systems/inventorysystem.cpp
  src/systems/partysystem.cpp
  src/systems/mapsystem.cpp
  )

add_executable(${PROJECT} ${SOURCES_MAIN} ${SOURCES_HEADER})

target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
set_target_properties(${PROJECT} PROPERTIES LINK_FLAGS "${${CMAKE_CXX_COMPILER_ID}_LINK_FLAGS}")
        
if(WITH_MYSQL)
  target_link_libraries(${PROJECT} ${MYSQL_LIBRARIES})
endif()

target_link_libraries(${PROJECT} RoseCommon)
target_link_libraries(${PROJECT} ${CMAKE_THREAD_LIBS_INIT})

if (MSVC)
  set_target_properties(${PROJECT} PROPERTIES COMPILE_FLAGS "/wd4503")
  target_link_libraries(${PROJECT} wsock32.lib)
else()
    if (WITH_MYSQL)
      target_link_libraries(${PROJECT} mysqlclient)
    endif()
endif()

if(WITH_CRASH_REPORTS)
  add_dependencies(${PROJECT} breakpad)
  target_link_libraries(${PROJECT} ${BREAKPAD_EXCEPTION_HANDLER_LIBRARIES})
endif()

set_target_properties(${PROJECT} PROPERTIES COTIRE_ENABLE_PRECOMPILED_HEADER TRUE)

generate_symbol_data(${PROJECT})
if (COMMAND cotire)
  cotire(${PROJECT})
endif()

SET_PROPERTY(TARGET ${PROJECT}                PROPERTY FOLDER "Servers")
