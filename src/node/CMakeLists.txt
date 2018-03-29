
link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT NodeServer)

project(${PROJECT})

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  add_compiler_flags(-Wl,-Map=${PROJECT}.map)
ENDIF()

SET( SOURCES_MAIN
  src/main.cpp )

SET( SOURCES_HEADER
  include/nodeserver
  include/nodeclient
  include/nodeisc )

SET( SOURCES_BASE
  src/nodeserver
  src/nodeclient
  src/nodeisc )
  
add_executable(${PROJECT} ${SOURCES_MAIN} ${SOURCES_BASE} ${SOURCES_HEADER})

target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
set_target_properties(${PROJECT} PROPERTIES LINK_FLAGS "${${CMAKE_CXX_COMPILER_ID}_LINK_FLAGS}")

target_link_libraries(${PROJECT} RoseCommon)
target_link_libraries(${PROJECT} ${CMAKE_THREAD_LIBS_INIT})

#if(UNIX)
#  target_link_libraries(${PROJECT} ${CURL_LIBRARIES})
#endif()

IF (MSVC)
  set_target_properties(${PROJECT} PROPERTIES COMPILE_FLAGS "/wd4503")
  target_link_libraries(${PROJECT} wsock32.lib)
ENDIF()

generate_symbol_data(${PROJECT})
