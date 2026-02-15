set(SPDLOG_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})
set(CMAKE_INSTALL_PREFIX ${CMAKE_THIRD_PARTY_DIR})
set(INSTALL_PLUGINDIR ${CMAKE_INSTALL_PREFIX})

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog
  GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(spdlog)