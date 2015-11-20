cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("Unit Tests")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 3rd Party Libaries
include_directories(BEFORE SYSTEM $ENV{BOOST_ROOT} 
			${CMAKE_HOME_DIRECTORY}/3rdParty/include
			${GTEST_DIR}/include 
			${GTEST_DIR} )
link_directories(${CMAKE_HOME_DIRECTORY}/3rdParty/lib)

# Our Stuff 
include_directories(${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_HOME_DIRECTORY}/Common/include )

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


message(STATUS "PROJECT_ROOT=" ${PROJECT_ROOT})
message(STATUS "COV_SRC_LIST=" ${COV_SRC_LIST})

# Setup the coveralls target and tell it to gather
# coverage data for all the lib sources.
if (COVERALLS)
    include(Coveralls)
    coveralls_turn_on_coverage()
    coveralls_setup(
        "${COV_SRC_LIST}"
        ${COVERALLS_UPLOAD}
        "${CMAKE_HOME_DIRECTORY}/tools/coveralls-cmake/cmake")
endif()

# tests
enable_testing()
add_test(RoseEncryptionTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TESTRoseEncryption)
#add_test(Test_XML_IO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TESTXmlIO)
#add_test(RoseEncryptionTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TEST<NAME>)
#add_test(RoseEncryptionTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TEST<NAME>)
