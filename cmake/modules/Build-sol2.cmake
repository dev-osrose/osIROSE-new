set(SOL2_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})
set(CMAKE_INSTALL_PREFIX ${CMAKE_THIRD_PARTY_DIR})
set(INSTALL_PLUGINDIR ${CMAKE_INSTALL_PREFIX})

FetchContent_Declare(
  sol2
  GIT_REPOSITORY https://github.com/ThePhD/sol2
  GIT_TAG v3.3.0
)

FetchContent_MakeAvailable(sol2)