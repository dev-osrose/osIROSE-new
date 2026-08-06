set(SOL2_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

# EXCLUDE_FROM_ALL suppresses sol2's install rules, which would otherwise drop
# its ~120 headers into our install prefix. See Build-sqlpp23.cmake for why
# CMAKE_SKIP_INSTALL_RULES is not the tool for this.
FetchContent_Declare(
  sol2
  GIT_REPOSITORY https://github.com/ThePhD/sol2
  GIT_TAG c1f95a773c6f8f4fde8ca3efe872e7286afe4444
  EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(sol2)