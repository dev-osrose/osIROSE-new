set(ENTT_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

# EXCLUDE_FROM_ALL suppresses EnTT's install rules, which would otherwise drop
# its headers plus a cmake package config and pkgconfig file into our install
# prefix. See Build-sqlpp23.cmake for why CMAKE_SKIP_INSTALL_RULES is not the
# tool for this.
FetchContent_Declare(
  entt
  GIT_REPOSITORY https://github.com/skypjack/entt
  GIT_TAG 2d5a3f24aa6dc38ed0c4eee9d4d625586d8e921e
  EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(entt)