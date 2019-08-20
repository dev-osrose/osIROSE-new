if(WIN32 OR MINGW)
ExternalProject_Add(
  gn
  GIT_REPOSITORY https://gn.googlesource.com/gn
  BUILD_IN_SOURCE TRUE
  DEPENDS utils::ninja
  
  CONFIGURE_COMMAND Python2::Interpreter build/gen.py
  BUILD_COMMAND utils::ninja -C out
  INSTALL_COMMAND ""
)
else()
ExternalProject_Add(
  gn
  GIT_REPOSITORY https://gn.googlesource.com/gn
  BUILD_IN_SOURCE TRUE
  DEPENDS utils::ninja
  
  CONFIGURE_COMMAND CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} Python2::Interpreter build/gen.py
  BUILD_COMMAND utils::ninja -C out
  INSTALL_COMMAND ""
)
endif()
ExternalProject_Get_Property(
  gn
  source_dir
)

if(WIN32 OR MINGW)
  set(GN_EXE_PATH ${source_dir}/out/gn.exe)
else()
  set(GN_EXE_PATH ${source_dir}/out/gn)
endif()

if(NOT TARGET utils::gn)
  add_executable(utils::gn IMPORTED GLOBAL)
  add_dependencies(utils::gn gn)
  set_target_properties(utils::gn PROPERTIES IMPORTED_LOCATION ${GN_EXE_PATH})
endif()

mark_as_advanced( GN_EXE_PATH )