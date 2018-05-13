
link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT LoginServer)

project(${PROJECT})

if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  add_compiler_flags(-Wl,-Map=${PROJECT}.map)
endif()

SET( SOURCES_MAIN
  src/main )

SET( SOURCES_HEADER
  include/cloginserver.h
  include/cloginclient.h
  include/cloginisc.h )

SET( SOURCES_BASE
  src/cloginserver.cpp
  src/cloginclient.cpp
  src/cloginisc.cpp )
  
add_executable(${PROJECT} ${SOURCES_MAIN} ${SOURCES_BASE} ${SOURCES_HEADER})

target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
set_target_properties(${PROJECT} PROPERTIES LINK_FLAGS "${${CMAKE_CXX_COMPILER_ID}_LINK_FLAGS}")

if(WITH_MYSQL)
  target_link_libraries(${PROJECT} ${MYSQL_LIBRARIES})
endif()

target_link_libraries(${PROJECT} RoseCommon)
target_link_libraries(${PROJECT} ${CMAKE_THREAD_LIBS_INIT})

if(UNIX)
  target_link_libraries(${PROJECT} ${CURL_LIBRARIES})
endif()

if (MSVC)
  set_target_properties(${PROJECT} PROPERTIES COMPILE_FLAGS "/wd4503")
  target_link_libraries(${PROJECT} wsock32.lib)
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
