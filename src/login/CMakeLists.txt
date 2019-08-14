
link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT LoginServer)

project(${PROJECT})

if (GCC)
  add_compiler_flags(-Wl,-Map=${PROJECT}.map)
  add_linker_flags(-Wl,-rpath -Wl,./)
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

target_include_directories(${PROJECT}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_dependencies(${PROJECT} RoseCommon)
target_link_libraries(${PROJECT}
  PRIVATE
    RoseCommon
)

if(WITH_CRASH_REPORTS)
  generate_symbol_data(${PROJECT})
#  generate_crashpad_symbol_data(${PROJECT})
endif()

install(TARGETS ${PROJECT}
        RUNTIME
          DESTINATION bin
          COMPONENT Runtime)

if (COMMAND cotire)
  cotire(${PROJECT})
endif()

SET_PROPERTY(TARGET ${PROJECT}                PROPERTY FOLDER "Servers")
