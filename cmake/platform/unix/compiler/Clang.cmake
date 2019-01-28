message("-- using clang")

add_compiler_flags(-std=c++17)
add_compiler_flags(-frelaxed-template-template-args)
add_compiler_flags(-Wno-keyword-compat -Wno-unused-command-line-argument)
add_linker_flags(-lpthread)

if(APPLE)
  add_compiler_flags(-ftemplate-depth=900 -Wno-macro-redefined)
else()
  add_compiler_flags(-ftemplate-depth=900 -Wno-expansion-to-defined)
endif()

set(CLANG TRUE)
