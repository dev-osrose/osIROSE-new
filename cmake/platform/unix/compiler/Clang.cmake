message("-- using clang")

add_compiler_flags(-Wno-keyword-compat -Wno-unused-command-line-argument)

if( ${CMAKE_CXX_COMPILER_ID} STREQUAL Clang )
  if(APPLE)
    add_compiler_flags(-ftemplate-depth=900 -Wno-macro-redefined)
  else()
    add_compiler_flags(-ftemplate-depth=900 -Wno-expansion-to-defined)
  endif()
endif()

set(CLANG TRUE)