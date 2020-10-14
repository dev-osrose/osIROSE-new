
link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT Bot)

project(${PROJECT})

#add_compiler_flags($<$<CXX_COMPILER_ID:Clang>:-Wno-keyword-compat>)

SET( SOURCES_MAIN
  src/main.cpp
  src/client.cpp
  src/login_client.cpp
  src/char_client.cpp
  src/map_client.cpp
)

SET( SOURCES_HEADER
  include/client.h
  include/login_client.h
  include/char_client.h
  include/map_client.h
)

SET( SOURCES_BASE
)

add_executable(${PROJECT} ${SOURCES_MAIN} ${SOURCES_BASE} ${SOURCES_HEADER})

target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_compile_options(${PROJECT} PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wno-packed-bitfield-compat -Wl,-Map=$<TARGET_NAME:${PROJECT}>.map>)

if(WITH_MYSQL)
  target_link_libraries(${PROJECT} ${MYSQL_LIBRARIES})
endif()

if(WITH_CRASH_REPORTS)
  if (NOT CRASHPAD)
    if(NOT Breakpad_FOUND)
      add_dependencies(${PROJECT} breakpad-build)
    endif()
  else()
    if(NOT CRASHPAD_FOUND)
      add_dependencies(${PROJECT} crashpad-build)
    endif()
  endif()
endif()

add_dependencies(${PROJECT} RoseCommon)
target_link_libraries(${PROJECT} RoseCommon osirose_common::osirose_common)
target_link_libraries(${PROJECT} ${CMAKE_THREAD_LIBS_INIT})

if (MSVC)
  set_target_properties(${PROJECT} PROPERTIES COMPILE_FLAGS "/wd4503")
  target_link_libraries(${PROJECT} wsock32.lib)
endif()

if (NOT CRASHPAD)
  generate_symbol_data(${PROJECT})
else()
  generate_crashpad_symbol_data(${PROJECT})
endif()

SET_PROPERTY(TARGET ${PROJECT}                PROPERTY FOLDER "Tools")
