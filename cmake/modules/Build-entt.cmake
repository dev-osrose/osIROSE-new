set(ENTT_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})
set(CMAKE_INSTALL_PREFIX ${CMAKE_THIRD_PARTY_DIR})
set(INSTALL_PLUGINDIR ${CMAKE_INSTALL_PREFIX})

FetchContent_Declare(
  entt
  GIT_REPOSITORY https://github.com/skypjack/entt
  GIT_TAG 2d5a3f24aa6dc38ed0c4eee9d4d625586d8e921e
)

FetchContent_MakeAvailable(entt)