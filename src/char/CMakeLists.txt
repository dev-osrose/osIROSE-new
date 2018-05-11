
link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT CharServer)
project(${PROJECT})

#set(MSVC_LINK_FLAGS " /NODEFAULTLIB:MSVCRTD /NODEFAULTLIB:MSVCRT")
#add_linker_flags(/NODEFAULTLIB:MSVCRTD /NODEFAULTLIB:MSVCRT)

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  add_compiler_flags(-Wl,-Map=${PROJECT}.map)
ENDIF()

set( SOURCES_MAIN
  src/main
)

set( SOURCES_HEADER
  include/ccharserver.h
  include/ccharclient.h
  include/ccharisc.h )

set( SOURCES_BASE
  src/ccharserver.cpp
  src/ccharclient.cpp
  src/ccharisc.cpp
)

add_executable(${PROJECT} ${SOURCES_MAIN} ${SOURCES_BASE} ${SOURCES_HEADER})

target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
set_target_properties(${PROJECT} PROPERTIES LINK_FLAGS "${${CMAKE_CXX_COMPILER_ID}_LINK_FLAGS}")
				
IF(WITH_MYSQL)
  target_link_libraries(${PROJECT} ${MYSQL_LIBRARIES})
ENDIF()

target_link_libraries(${PROJECT} RoseCommon)
target_link_libraries(${PROJECT} ${CMAKE_THREAD_LIBS_INIT})

IF (MSVC)
  set_target_properties(${PROJECT} PROPERTIES COMPILE_FLAGS "/wd4503")
  target_link_libraries(${PROJECT} wsock32.lib)
ELSE()
  IF (WITH_MYSQL)
      target_link_libraries(${PROJECT} mysqlclient)
  ENDIF()
ENDIF()

generate_symbol_data(${PROJECT})
if (COMMAND cotire)
  cotire(${PROJECT})
endif()

SET_PROPERTY(TARGET ${PROJECT}                PROPERTY FOLDER "Servers")
