# Only do this for windows platforms
if(WIN32 OR MINGW)
  message(STATUS "Building sqlite3 since sqlite3 was not found")

  include(FetchContent)
  FetchContent_Declare(
          sqlite3
          URL https://www.sqlite.org/2023/sqlite-amalgamation-3430100.zip
          URL_HASH SHA3_256=2110f85a90033016f5b5691812448e8dfa606fe407f4b2ebfec66dda2968fe3c
          DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  )

  FetchContent_MakeAvailable(sqlite3)

  # Build sqlite
  add_library(SQLite3 STATIC
          ${sqlite3_SOURCE_DIR}/sqlite3.c
  )

  target_include_directories(SQLite3 PUBLIC
          ${sqlite3_SOURCE_DIR}
  )

  # Also provide namespaced alias (some projects expect this)
  if(NOT TARGET SQLite::SQLite3)
    add_library(SQLite::SQLite3 ALIAS SQLite3)
  endif()

  # Make find_package(SQLite3) think it succeeded
  set(SQLite3_FOUND TRUE CACHE BOOL "" FORCE)
  set(SQLite3_LIBRARIES SQLite3 CACHE STRING "" FORCE)
  set(SQLite3_INCLUDE_DIR ${sqlite3_SOURCE_DIR} CACHE STRING "" FORCE)
  set(SQLite3_LIBRARY SQLite3 CACHE STRING "" FORCE)
endif()