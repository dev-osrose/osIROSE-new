
link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT RoseCommon)
project(${PROJECT})

set( SOURCES_HEADER
  include/crosesocket.h
  include/croseserver.h
  include/croseclient.h
  include/croseisc.h
  include/itemdb.h
)

SET(SOURCES_BASE
  src/crosesocket.cpp
  src/croseserver.cpp
  src/croseclient.cpp
  src/croseisc.cpp
  src/itemdb.cpp
)

add_library(${PROJECT} STATIC ${SOURCES_BASE} ${SOURCES_HEADER})
target_include_directories(${PROJECT}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

if (NOT MSVC)
  target_compile_options(${PROJECT} PRIVATE -Werror)
endif()

add_dependencies(${PROJECT} Core)
add_dependencies(${PROJECT} Core osirose_common::osirose_common)
target_link_libraries(${PROJECT}
  PUBLIC
    Core
    osirose_common::osirose_common
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source Files" FILES ${SOURCES_BASE})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include PREFIX "Header Files" FILES ${SOURCES_HEADER})

if (COMMAND cotire)
  cotire(${PROJECT})
endif()

SET_PROPERTY(TARGET ${PROJECT}                PROPERTY FOLDER "Servers")
