message("-- using GCC")

add_compiler_flags(DEBUG -O0 -fno-omit-frame-pointer -g3)
add_compiler_flags(RELEASE -O2)
add_compile_options(-Wno-packed-bitfield-compat)

add_compile_options(-pipe)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  if( ENABLE_ASAN )
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
  elseif( ENABLE_LSAN )
    add_compile_options(-fsanitize=leak -fno-omit-frame-pointer)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=leak")
  endif()

  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
endif()

if (WITH_COVERAGE_REPORTS)
  add_compile_options(--coverage)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov --coverage")
endif()

set(GCC TRUE)