
set(_byproducts
  <SOURCE_DIR>/gclient.bat
  <SOURCE_DIR>/gclient
  <SOURCE_DIR>/fetch.bat
  <SOURCE_DIR>/fetch
  <SOURCE_DIR>/ninja.exe
  <SOURCE_DIR>/ninja
)

if(WIN32 OR MINGW)
ExternalProject_Add(
  depot_tools
  URL https://storage.googleapis.com/chrome-infra/depot_tools.zip
  #GIT_REPOSITORY https://chromium.googlesource.com/chromium/tools/depot_tools.git
  BUILD_IN_SOURCE TRUE
  BUILD_BYPRODUCTS ${_byproducts}
  
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND <SOURCE_DIR>/gclient.bat
)
else()
ExternalProject_Add(
  depot_tools
  GIT_REPOSITORY https://chromium.googlesource.com/chromium/tools/depot_tools.git
  BUILD_IN_SOURCE TRUE
  BUILD_BYPRODUCTS ${_byproducts}
  
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
)
endif()
ExternalProject_Get_Property(
  depot_tools
  source_dir
)

set(ENV{PATH} "${source_dir};$ENV{PATH}")

set(DEPOT_TOOLS_PATH ${source_dir})
if(WIN32 OR MINGW)
  set(GCLIENT_EXE_PATH ${source_dir}/gclient.bat)
  set(NINJA_EXE_PATH ${source_dir}/ninja.exe)
  set(FETCH_EXE_PATH ${source_dir}/fetch.bat)
else()
  set(GCLIENT_EXE_PATH ${source_dir}/gclient)
  set(NINJA_EXE_PATH ${source_dir}/ninja)
  set(FETCH_EXE_PATH ${source_dir}/fetch)
endif()

if(NOT TARGET utils::fetch)
  add_executable(utils::fetch IMPORTED GLOBAL)
  add_dependencies(utils::fetch depot_tools)
  set_target_properties(utils::fetch PROPERTIES IMPORTED_LOCATION ${FETCH_EXE_PATH})
endif()

if(NOT TARGET utils::gclient)
  add_executable(utils::gclient IMPORTED GLOBAL)
  add_dependencies(utils::gclient depot_tools)
  set_target_properties(utils::gclient PROPERTIES IMPORTED_LOCATION ${GCLIENT_EXE_PATH})
endif()

if(NOT TARGET utils::ninja)
  add_executable(utils::ninja IMPORTED GLOBAL)
  add_dependencies(utils::ninja depot_tools)
  set_target_properties(utils::ninja PROPERTIES IMPORTED_LOCATION ${NINJA_EXE_PATH})
endif()

mark_as_advanced( DEPOT_TOOLS_PATH FETCH_EXE_PATH GCLIENT_EXE_PATH NINJA_EXE_PATH )