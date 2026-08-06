set(CXXOPTS_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts
  GIT_TAG v3.3.1
)

FetchContent_MakeAvailable(cxxopts)