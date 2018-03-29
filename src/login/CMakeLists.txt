
link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT LoginServer)

project(${PROJECT})

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  add_compiler_flags(-Wl,-Map=${PROJECT}.map)
ENDIF()

SET( SOURCES_MAIN
  src/main.cpp )

SET( SOURCES_HEADER
  include/cloginserver
  include/cloginclient
  include/cloginisc )

SET( SOURCES_BASE
  src/cloginserver
  src/cloginclient
  src/cloginisc )
  
add_executable(${PROJECT} ${SOURCES_MAIN} ${SOURCES_BASE} ${SOURCES_HEADER})

target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
set_target_properties(${PROJECT} PROPERTIES LINK_FLAGS "${${CMAKE_CXX_COMPILER_ID}_LINK_FLAGS}")

IF(WITH_MYSQL)
  target_link_libraries(${PROJECT} ${MYSQL_LIBRARIES})
ENDIF()

target_link_libraries(${PROJECT} RoseCommon)
target_link_libraries(${PROJECT} ${CMAKE_THREAD_LIBS_INIT})

if(UNIX)
  target_link_libraries(${PROJECT} ${CURL_LIBRARIES})
endif()

IF (MSVC)
  set_target_properties(${PROJECT} PROPERTIES COMPILE_FLAGS "/wd4503")
  target_link_libraries(${PROJECT} wsock32.lib)
ENDIF()

generate_symbol_data(${PROJECT})
