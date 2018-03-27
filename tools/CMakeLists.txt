CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)
PROJECT(tools)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if(BUILD_TOOLS)
  INCLUDE_DIRECTORIES(
    SYSTEM ${SPDLOG_ROOT}
  )

  if(ENABLE_TESTING AND ENABLE_TESTING_COVERAGE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fprofile-arcs -ftest-coverage")
  endif() # ENABLE_TESTING_COVERAGE

  if( ENABLE_ASAN )
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address -fno-omit-frame-pointer")
    SET(LINK_FLAGS "${LINK_FLAGS} -g -fsanitize=address")
  endif()

  if(BUILD_STRESS)
	  ADD_SUBDIRECTORY(stress)
  endif()
endif()
