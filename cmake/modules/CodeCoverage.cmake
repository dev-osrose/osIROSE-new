# Copyright (c) 2012 - 2017, Lars Bilke
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its contributors
#    may be used to endorse or promote products derived from this software without
#    specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# CHANGES:
#
# 2012-01-31, Lars Bilke
# - Enable Code Coverage
#
# 2013-09-17, Joakim Söderberg
# - Added support for Clang.
# - Some additional usage instructions.
#
# 2016-02-03, Lars Bilke
# - Refactored functions to use named parameters
#
# 2017-06-02, Lars Bilke
# - Merged with modified version from github.com/ufz/ogs
#
#
# USAGE:
#
# 1. Copy this file into your cmake modules path.
#
# 2. Add the following line to your CMakeLists.txt:
#      include(CodeCoverage)
#
# 3. Append necessary compiler flags:
#      APPEND_COVERAGE_COMPILER_FLAGS()
#
# 4. If you need to exclude additional directories from the report, specify them
#    using the COVERAGE_EXCLUDES variable before calling SETUP_TARGET_FOR_COVERAGE.
#    Example:
#      set(COVERAGE_EXCLUDES 'dir1/*' 'dir2/*')
#
# 5. Use the functions described below to create a custom make target which
#    runs your test executable and produces a code coverage report.
#
# 6. Build a Debug build:
#      cmake -DCMAKE_BUILD_TYPE=Debug ..
#      make
#      make my_coverage_target
#

include(CMakeParseArguments)

IF(DEFINED ENV{GCOV_PATH})
    SET(GCOV_PATH $ENV{GCOV_PATH})
ENDIF()

IF(DEFINED ENV{LCOV_PATH})
    SET(LCOV_PATH $ENV{LCOV_PATH})
ENDIF()

IF(DEFINED ENV{GENHTML_PATH})
    SET(GENHTML_PATH $ENV{GENHTML_PATH})
ENDIF()

IF(DEFINED ENV{GCOVR_PATH})
    SET(GCOVR_PATH $ENV{GCOVR_PATH})
ENDIF()

IF(DEFINED ENV{GCOV_EXEC})
  SET(GCOV_EXEC $ENV{GCOV_EXEC})
ELSE()
  SET(GCOV_EXEC gcov)
ENDIF()

# Check prereqs
IF(NOT GCOV_PATH)
  FIND_PROGRAM( GCOV_PATH ${GCOV_EXEC} )
ENDIF()

IF(NOT LCOV_PATH)
  find_program( LCOV_PATH NAMES lcov lcov.bat lcov.exe lcov.perl)
ENDIF()

IF(NOT GENHTML_PATH)
  find_program( GENHTML_PATH NAMES genhtml genhtml.perl genhtml.bat )
ENDIF()

IF(NOT GCOVR_PATH)
  find_program( GCOVR_PATH gcovr PATHS ${CMAKE_SOURCE_DIR}/scripts/test)
ENDIF()

IF(NOT GCOV_PATH)
	MESSAGE(FATAL_ERROR "gcov not found! Aborting...")
ENDIF() # NOT GCOV_PATH

IF("${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
	IF("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 3)
		MESSAGE(FATAL_ERROR "Clang version must be 3.0.0 or greater! Aborting...")
	ENDIF()
ELSEIF(NOT CMAKE_COMPILER_IS_GNUCXX)
	MESSAGE(FATAL_ERROR "Compiler is not GNU gcc! Aborting...")
ENDIF() # CHECK VALID COMPILER

SET(CMAKE_CXX_FLAGS_COVERAGE
    "-g -O0 --coverage -fprofile-arcs -ftest-coverage"
    CACHE STRING "Flags used by the C++ compiler during coverage builds."
    FORCE )
SET(CMAKE_C_FLAGS_COVERAGE
    "-g -O0 --coverage -fprofile-arcs -ftest-coverage"
    CACHE STRING "Flags used by the C compiler during coverage builds."
    FORCE )
SET(CMAKE_EXE_LINKER_FLAGS_COVERAGE
    ""
    CACHE STRING "Flags used for linking binaries during coverage builds."
    FORCE )
SET(CMAKE_SHARED_LINKER_FLAGS_COVERAGE
    ""
    CACHE STRING "Flags used by the shared libraries linker during coverage builds."
    FORCE )
MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_COVERAGE
    CMAKE_C_FLAGS_COVERAGE
    CMAKE_EXE_LINKER_FLAGS_COVERAGE
    CMAKE_SHARED_LINKER_FLAGS_COVERAGE )

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(WARNING "Code coverage results with an optimised (non-Debug) build may be misleading")
endif() # NOT CMAKE_BUILD_TYPE STREQUAL "Debug"

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    link_libraries(gcov)
else()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()


# Defines a target for running and collection code coverage information
# Builds dependencies, runs the given executable and outputs reports.
# NOTE! The executable should always have a ZERO as exit code otherwise
# the coverage generation will not complete.
#
# SETUP_TARGET_FOR_COVERAGE(
#     NAME testrunner_coverage                    # New target name
#     EXECUTABLE testrunner -j ${PROCESSOR_COUNT} # Executable in PROJECT_BINARY_DIR
#     DEPENDENCIES testrunner                     # Dependencies to build first
# )
function(SETUP_TARGET_FOR_COVERAGE)

  set(options NONE)
  set(oneValueArgs NAME)
  set(multiValueArgs EXECUTABLE EXECUTABLE_ARGS DEPENDENCIES EXCLUDES)
  cmake_parse_arguments(Coverage "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(NOT LCOV_PATH)
    message(FATAL_ERROR "lcov not found! Aborting...")
  endif() # NOT LCOV_PATH

  if(NOT GENHTML_PATH)
    message(FATAL_ERROR "genhtml not found! Aborting...")
  endif() # NOT GENHTML_PATH

  # Setup target
  add_custom_target(${Coverage_NAME}

    # Cleanup lcov
    COMMAND ${LCOV_PATH} --directory . --zerocounters
    # Create baseline to make sure untouched files show up in the report
    COMMAND ${LCOV_PATH} -c -i -d . -o ${Coverage_NAME}.base

    # Run tests
    COMMAND ${Coverage_EXECUTABLE}

    # Capturing lcov counters and generating report
    COMMAND ${LCOV_PATH} --directory . --capture --output-file ${Coverage_NAME}.info
    # add baseline counters
    COMMAND ${LCOV_PATH} -a ${Coverage_NAME}.base -a ${Coverage_NAME}.info --output-file ${Coverage_NAME}.total
    COMMAND ${LCOV_PATH} --remove ${Coverage_NAME}.total ${Coverage_EXCLUDES} --output-file ${PROJECT_BINARY_DIR}/${Coverage_NAME}.info.cleaned
    COMMAND ${GENHTML_PATH} -o ${Coverage_NAME} ${PROJECT_BINARY_DIR}/${Coverage_NAME}.info.cleaned
    COMMAND ${CMAKE_COMMAND} -E remove ${Coverage_NAME}.base ${Coverage_NAME}.total ${PROJECT_BINARY_DIR}/${Coverage_NAME}.info.cleaned

    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    DEPENDS ${Coverage_DEPENDENCIES}
    COMMENT "Resetting code coverage counters to zero.\nProcessing code coverage counters and generating report."
  )
  
  # Show where to find the lcov info report
  add_custom_command(TARGET ${Coverage_NAME} POST_BUILD
    COMMAND ;
    COMMENT "Lcov code coverage info report saved in ${Coverage_NAME}.info."
  )

  # Show info where to find the report
  add_custom_command(TARGET ${Coverage_NAME} POST_BUILD
    COMMAND ;
    COMMENT "Open ./${Coverage_NAME}/index.html in your browser to view the coverage report."
  )

endfunction() # SETUP_TARGET_FOR_COVERAGE



# Param _targetname     The name of new the custom make target
# Param _testrunner     The name of the target which runs the tests.
#					MUST return ZERO always, even on errors.
#					If not, no coverage report will be created!
# Param _outputname     lcov output is generated as _outputname.info
#                       HTML report is generated in _outputname/index.html
# Optional fourth parameter is passed as arguments to _testrunner
#   Pass them in list form, e.g.: "-j;2" for -j 2
#FUNCTION(SETUP_TARGET_FOR_COVERAGE _targetname _testrunner _outputname)
#
#  IF(NOT GCOV_PATH)
#		MESSAGE(FATAL_ERROR "gcov not found! Aborting...")
#	ENDIF() # NOT GCOV_PATH
#
#	IF(NOT LCOV_PATH)
#		MESSAGE(FATAL_ERROR "lcov not found! Aborting...")
#	ENDIF() # NOT LCOV_PATH
#
#	IF(NOT GENHTML_PATH)
#		MESSAGE(FATAL_ERROR "genhtml not found! Aborting...")
#	ENDIF() # NOT GENHTML_PATH
#
#	SET(coverage_info "${CMAKE_BINARY_DIR}/${_outputname}.info")
#	SET(coverage_cleaned "${coverage_info}.cleaned")
#
#	SEPARATE_ARGUMENTS(test_command UNIX_COMMAND "${_testrunner}")
#	
#	add_custom_command( OUTPUT tmp1.output
#		COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} --directory . --zerocounters
#		COMMAND ${CMAKE_COMMAND} -E touch tmp1.output
#		
#		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#		COMMENT "Resetting code coverage counters to zero."
#	)
#	
#	add_custom_command( OUTPUT tmp2.output
#		COMMAND ${test_command} ${ARGV3}
#		COMMAND ${CMAKE_COMMAND} -E touch tmp2.output
#		
#		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#		COMMENT "Running tests."
#		DEPENDS tmp1.output
#	)
#	
#	add_custom_command( OUTPUT tmp3.output
#		COMMAND ${CMAKE_COMMAND} -E touch tmp3.output
#		
#		# Capturing lcov counters and generating report
#		COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} --directory . --capture --output-file ${coverage_info}
#		COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} --remove ${coverage_info} 'tests/*' '/usr/*' ${COVERAGE_IGNORE_LIST} --output-file ${coverage_cleaned}
#		COMMAND ${GENHTML_PATH} -o ${_outputname} ${coverage_cleaned}
#		
#		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#		COMMENT "Processing code coverage counters and generating report."
#		DEPENDS tmp2.output
#	)
#	
#	add_custom_target(${_targetname} 
#		DEPENDS tmp3.output
#	)
#
#	# Setup target
##	add_custom_target(${_targetname}
##
##		# Cleanup lcov
##		${LCOV_PATH} --gcov-tool ${GCOV_PATH} --directory . --zerocounters
##
##		# Run tests
##		COMMAND ${test_command} ${ARGV3}
##
##		# Capturing lcov counters and generating report
##		COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} --directory . --capture --output-file ${coverage_info}
##		COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} --remove ${coverage_info} 'tests/*' '/usr/*' ${COVERAGE_IGNORE_LIST} --output-file ${coverage_cleaned}
##		COMMAND ${GENHTML_PATH} -o ${_outputname} ${coverage_cleaned}
##
##		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
##		VERBATIM
##		COMMENT "Resetting code coverage counters to zero.\nProcessing code coverage counters and generating report."
##	)
#
#	# Show info where to find the report
#	add_custom_command(TARGET ${_targetname} POST_BUILD
#		COMMAND ;
#		COMMENT "Open ./${_outputname}/index.html in your browser to view the coverage report."
#	)
#
#ENDFUNCTION() # SETUP_TARGET_FOR_COVERAGE