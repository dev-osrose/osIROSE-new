message("-- using GCC")

add_compiler_flags(DEBUG -fno-omit-frame-pointer -g3)
add_compiler_flags(RELEASE -O2)
add_compiler_flags(CXX -std=c++17)
add_compiler_flags(-Wno-packed-bitfield-compat)

add_compiler_flags(-pipe)

if(DEBUG)
  add_compiler_flags(-O0)
  
  IF( ENABLE_ASAN )
    add_compiler_flags(-fsanitize=address -fno-omit-frame-pointer)
    add_linker_flags(-fsanitize=address)
  ENDIF()
  add_linker_flags(-rdynamic)
endif()

if (WITH_COVERAGE_REPORTS)
  add_compiler_flags(--coverage)
endif()

set(GCC TRUE)