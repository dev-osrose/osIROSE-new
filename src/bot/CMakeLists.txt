
link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT Bot)

project(${PROJECT})

if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  add_compiler_flags(-Wno-packed-bitfield-compat -Wl,-Map=${PROJECT}.map)
elseif( ${CMAKE_CXX_COMPILER_ID} STREQUAL Clang )
  add_compiler_flags(-Wno-keyword-compat)
endif()

SET( SOURCES_MAIN
  src/main.cpp )
  
SET( SOURCES_HEADER
    )

SET( SOURCES_BASE
    )

add_executable(${PROJECT} ${SOURCES_MAIN} ${SOURCES_BASE} ${SOURCES_HEADER})

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
endif()

generate_symbol_data(${PROJECT})

SET_PROPERTY(TARGET ${PROJECT}                PROPERTY FOLDER "Tools")
