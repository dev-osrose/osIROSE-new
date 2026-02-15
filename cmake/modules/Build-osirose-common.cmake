include(FetchContent)

FetchContent_Declare(
    osirose_common
    GIT_REPOSITORY https://github.com/dev-osrose/packets
    GIT_TAG idl-module-updates
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