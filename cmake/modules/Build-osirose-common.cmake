include(FetchContent)

# ---------------------------------------------------------------------------
# Forward ENABLE_SSL into the packets sub-project so that osIROSE-common-core
# compiles with the matching USE_SSL preprocessor definition.
# Must be set as CACHE BOOL FORCE before FetchContent_MakeAvailable so that
# the sub-project's own option() declaration picks it up.
# ---------------------------------------------------------------------------
if(ENABLE_SSL)
  set(ENABLE_SSL ON  CACHE BOOL "Enable TLS/SSL in osIROSE-common-core" FORCE)
else()
  set(ENABLE_SSL OFF CACHE BOOL "Enable TLS/SSL in osIROSE-common-core" FORCE)
endif()

# ---------------------------------------------------------------------------
# Fetch the packets project.
# For local development you can bypass the git download by setting:
#   -DFETCHCONTENT_SOURCE_DIR_OSIROSE_COMMON=/path/to/local/packets
# on the CMake command line.
# ---------------------------------------------------------------------------
# SYSTEM demotes this project's headers to -isystem, so warnings coming out of
# vendored third-party code (json.hpp) do not trip our own -Werror.
FetchContent_Declare(
    osirose_common
    GIT_REPOSITORY https://github.com/dev-osrose/packets
    GIT_TAG idl-module-updates+ssl
    SYSTEM
)

FetchContent_MakeAvailable(osirose_common)

# Create a clean umbrella interface target
if(NOT TARGET osirose_common)
  add_library(osirose_common INTERFACE)

  target_link_libraries(osirose_common
      INTERFACE
      osIROSE-common-core
      osIROSE-common-rosecommon
  )

  add_library(osirose_common::osirose_common
      ALIAS osirose_common
  )
endif()