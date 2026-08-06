message("-- using clang")

add_compile_options(-pthread)
add_compile_options(-Wno-keyword-compat -Wno-unused-command-line-argument)

if(APPLE)
  add_compile_options(-ftemplate-depth=900 -Wno-macro-redefined)
else()
  add_compile_options(-ftemplate-depth=900 -Wno-expansion-to-defined)
endif()

set(CLANG TRUE)
