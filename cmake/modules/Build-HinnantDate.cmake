set(DATE_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

ExternalProject_Add(
  date
  GIT_REPOSITORY https://github.com/HowardHinnant/date.git
  GIT_TAG 3e82a52d665cdaaa51027f15906bf503b6896142
  CMAKE_ARGS -G ${CMAKE_GENERATOR} -DCMAKE_INSTALL_PREFIX=${DATE_INSTALL_DIR} -DENABLE_DATE_TESTING=OFF
  INSTALL_DIR ${DATE_INSTALL_DIR}
  BUILD_COMMAND cmake --build . --target install
)

ExternalProject_Get_Property(
  date
  install_dir
)

set(DATE_INCLUDE_DIR "${install_dir}/include")
if(NOT TARGET DATE::DATE)
    add_library(DATE::DATE INTERFACE IMPORTED)
    add_dependencies(DATE::DATE date)
    set_target_properties(DATE::DATE PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${DATE_INCLUDE_DIR}")
endif()