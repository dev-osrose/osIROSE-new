#message(STATUS "Checking for working template compiler")
#try_compile(HAVE_TEMPLATES17 ${CMAKE_BINARY_DIR} SOURCES ${CMAKE_TESTS_PATH}/templates17_test.cpp
#  CXX_STANDARD 17
#  CXX_STANDARD_REQUIRED true
#  CXX_EXTENSIONS OFF
#)

#if(NOT ${HAVE_TEMPLATES17})
#  message(FATAL_ERROR "Compiler is not compatible with our template usage.")
#endif()

message(STATUS "Checking if compiler supports sqlpp11")
try_compile(HAVE_SQLPP11_TEMPLATE_SUPPORT ${CMAKE_BINARY_DIR} SOURCES ${CMAKE_TESTS_PATH}/sqlpp11_template_input_test.cpp
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED true
  CXX_EXTENSIONS OFF
)

if(NOT ${HAVE_SQLPP11_TEMPLATE_SUPPORT})
  message(STATUS "Compiler is not compatible with default sqlpp11 trying patched version.")
  try_compile(HAVE_SQLPP11_TEMPLATE_SUPPORT_PATCHED ${CMAKE_BINARY_DIR} SOURCES ${CMAKE_TESTS_PATH}/sqlpp11_template_input_test_patched.cpp
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED true
    CXX_EXTENSIONS OFF
  )

  if(NOT ${HAVE_SQLPP11_TEMPLATE_SUPPORT_PATCHED})  
    message(FATAL_ERROR "Compiler is not compatible with sqlpp11.")
  endif()
endif()
