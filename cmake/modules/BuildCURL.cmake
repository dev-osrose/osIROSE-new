include(FetchContent)

# Prefer system curl first (optional but recommended)
find_package(CURL QUIET)

if(NOT CURL_FOUND)

  FetchContent_Declare(
    curl
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG curl-8_18_0
  )

  # Configure curl options BEFORE MakeAvailable
  set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
  set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)
  set(HTTP_ONLY ON CACHE BOOL "" FORCE)
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE) # static (recommended)
  set(CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)

  FetchContent_MakeAvailable(curl)

  # curl defines different targets depending on version
  # Normalize to CURL::libcurl
  if(TARGET libcurl AND NOT TARGET CURL::libcurl)
    add_library(CURL::libcurl ALIAS libcurl)
  endif()

  # Satisfy legacy FindCURL users
  set(CURL_FOUND TRUE CACHE BOOL "" FORCE)
  set(CURL_LIBRARIES CURL::libcurl CACHE STRING "" FORCE)

endif()