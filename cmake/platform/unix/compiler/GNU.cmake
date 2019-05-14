message("-- using GCC")

add_compiler_flags(DEBUG -fno-omit-frame-pointer -g3)
add_compiler_flags(RELEASE -O2)
add_compiler_flags(CXX -std=c++17)
add_compiler_flags(-Wno-packed-bitfield-compat)

add_compiler_flags(-pipe)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  add_compiler_flags(-O0)
  
  if( ENABLE_ASAN )
    add_compiler_flags(-fsanitize=address -fno-omit-frame-pointer)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
  elseif( ENABLE_LSAN )
    add_compiler_flags(-fsanitize=leak -fno-omit-frame-pointer)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=leak")
  endif()
  
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
endif()

if (WITH_COVERAGE_REPORTS)
  add_compiler_flags(--coverage)
endif()

set(GCC TRUE)