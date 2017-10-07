
link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT Bot)

project(${PROJECT})

#set(MSVC_LINK_FLAGS " /NODEFAULTLIB:MSVCRTD /NODEFAULTLIB:MSVCRT")
#add_linker_flags(/NODEFAULTLIB:MSVCRTD /NODEFAULTLIB:MSVCRT)

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  add_compiler_flags(-Wno-packed-bitfield-compat -Wl,-Map=${PROJECT}.map)
ELSEIF( ${CMAKE_CXX_COMPILER_ID} STREQUAL Clang )
  add_compiler_flags(-Wno-keyword-compat)
ENDIF()

SET( SOURCES_MAIN
  src/main.cpp )
  
SET( SOURCES_HEADER
    )

SET( SOURCES_BASE
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
  target_link_libraries(${PROJECT} wsock32.lib)
ENDIF()

install_executable(${PROJECT})
