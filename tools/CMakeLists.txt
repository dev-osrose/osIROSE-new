cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(tools)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if(BUILD_TOOLS)
  include_directories(
    SYSTEM ${SPDLOG_ROOT}
  )

  if(BUILD_TESTS AND WITH_COVERAGE_REPORTS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fprofile-arcs -ftest-coverage")
  endif() # WITH_COVERAGE_REPORTS

  if( ENABLE_ASAN )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address -fno-omit-frame-pointer")
    set(LINK_FLAGS "${LINK_FLAGS} -g -fsanitize=address")
  endif()

  if(BUILD_STRESS)
	  add_subdirectory(stress)
  endif()
endif()
