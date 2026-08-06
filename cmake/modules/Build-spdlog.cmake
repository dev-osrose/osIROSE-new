set(SPDLOG_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog
  GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(spdlog)