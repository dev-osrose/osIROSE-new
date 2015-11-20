cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("Unit Tests")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(std "")
	set(warnings "")
	set(errors "")
	set(defines "")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(std "")
	set(warnings "")
	set(errors "")
	set(defines "")
endif()

set(CMAKE_CXX_FLAGS "$(CMAKE_CXX_FLAGS} ${std} ${defines} ${warnings} ${errors}")

# 3rd Party Libaries
include_directories(BEFORE SYSTEM $ENV{BOOST_ROOT} 
			${CMAKE_HOME_DIRECTORY}/3rdParty/include
			${GTEST_DIR}/include)
link_directories(${CMAKE_HOME_DIRECTORY}/3rdParty/lib)

# Our Stuff 
include_directories(${GTEST_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

# Build Gtest
add_library(gtest STATIC ${GTEST_DIR}/src/gtest-all.cc)
add_library(gtest_main STATIC ${GTEST_DIR}/src/gtest_main.cc)
target_link_libraries(gtest_main gtest -pthread)

list( APPEND UNIT_LIST_SRC
		main.cpp )

find_package( Threads REQUIRED )
add_executable(TESTRoseEncryption ${UNIT_LIST_SRC} ./src/test_crosecrypt.cpp)
target_link_libraries(TESTRoseEncryption ${CMAKE_THREAD_LIBS_INIT} gtest_main)

#add_executable(TESTXmlIO ${UNIT_LIST_SRC} ./src/test_xml.cpp)
#target_link_libraries(TESTXmlIO ${CMAKE_THREAD_LIBS_INIT} gtest_main)

# tests
enable_testing()
add_test(RoseEncryptionTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TESTRoseEncryption)
#add_test(Test_XML_IO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TESTXmlIO)
#add_test(RoseEncryptionTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TEST<NAME>)
#add_test(RoseEncryptionTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TEST<NAME>)

set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")
