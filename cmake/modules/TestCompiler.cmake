message(STATUS "Checking for working template compiler")
try_compile(HAVE_TEMPLATES17 ${CMAKE_BINARY_DIR} SOURCES ${CMAKE_TESTS_PATH}/templates17_test.cpp
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED true
  CXX_EXTENSIONS OFF
)

if(NOT ${HAVE_TEMPLATES17})
  message(FATAL_ERROR "Compiler is not compatible with our template usage.")
endif()
