cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

function(message)
  list(GET ARGV 0 MessageType)
  if(MessageType STREQUAL FATAL_ERROR OR
     MessageType STREQUAL SEND_ERROR OR
     MessageType STREQUAL WARNING OR
     MessageType STREQUAL AUTHOR_WARNING)
    list(REMOVE_AT ARGV 0)
    _message(${MessageType} "${ARGV}")
  endif()
endfunction()

project("osIROSE")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Add project cmake modules to path.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/coveralls-cmake/cmake)

# We probably don't want this to run on every build.
option(COVERALLS "Turn on coveralls support" OFF)
option(COVERALLS_UPLOAD "Upload the generated coveralls json" ON)

if (COVERALLS)
    include(Coveralls)
    coveralls_turn_on_coverage()
endif()



message(STATUS "CXX=" $ENV{CXX})
message(STATUS "CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID}")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(std "-std=c++11")
	set(warnings "-Wall -Wextra -Werror -Wno-write-strings -Wno-unknown-pragmas")
	set(errors "")
	set(defines "")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(std "")
	set(warnings "/EHsc")
	set(errors "")
	set(defines "")
endif()

if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${std} ${defines} ${warnings} ${errors}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()


add_subdirectory(Common)
add_subdirectory("Login Server")
add_subdirectory("Char Server")
add_subdirectory("World Server")

# Setup the coveralls target and tell it to gather
# coverage data for all the lib sources.
if (COVERALLS)
        coveralls_setup(
        "${COMMON_LIST_SRC} ${LOGIN_LIST_SRC} ${CHAR_LIST_SRC} ${WORLD_LIST_SRC}"
        ${COVERALLS_UPLOAD}
        "${CMAKE_HOME_DIRECTORY}/coveralls-cmake/cmake")
endif()

#enable_testing()

#add_executable(complicated_regress ${PROJECT_SOURCE_DIR}/test/TEST_complicated.c)
#add_test(test_the_test ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/LoginServer)

set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")
