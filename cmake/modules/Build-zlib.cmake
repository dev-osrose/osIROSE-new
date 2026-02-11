find_package(ZLIB QUIET)

if(NOT ZLIB_FOUND)
  include(FetchContent)

  FetchContent_Declare(
    zlib
    URL https://www.zlib.net/current/zlib.tar.gz
  )

  set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(zlib)

  if(TARGET zlibstatic)
    add_library(ZLIB::ZLIB ALIAS zlibstatic)
  elseif(TARGET zlib)
    add_library(ZLIB::ZLIB ALIAS zlib)
  endif()

  set(ZLIB_FOUND TRUE CACHE BOOL "" FORCE)
endif()