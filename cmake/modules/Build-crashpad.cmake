set(CRASHPAD_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

set(_byproducts
  ${CRASHPAD_INSTALL_DIR}/lib/libbase.a
  ${CRASHPAD_INSTALL_DIR}/lib/libutil.a
  ${CRASHPAD_INSTALL_DIR}/lib/libclient.a
  ${CRASHPAD_INSTALL_DIR}/lib/libhandler.a
  ${CRASHPAD_INSTALL_DIR}/lib/libminidump.a
  ${CRASHPAD_INSTALL_DIR}/lib/libbase.lib
  ${CRASHPAD_INSTALL_DIR}/lib/libutil.lib
  ${CRASHPAD_INSTALL_DIR}/lib/libclient.lib
  ${CRASHPAD_INSTALL_DIR}/lib/libhandler.lib
  ${CRASHPAD_INSTALL_DIR}/lib/libminidump.lib
)

if(WIN32)
set(PATH $ENV{PATH})

ExternalProject_Add(
  crashpad
  BUILD_BYPRODUCTS ${_byproducts}
  BUILD_IN_SOURCE TRUE
  DEPENDS utils::gn utils::fetch utils::gclient utils::ninja
  STEP_TARGETS build install

  #DOWNLOAD_COMMAND Python2::Interpreter ${DEPOT_TOOLS_PATH}/fetch.py --no-history --force crashpad
  DOWNLOAD_COMMAND ${CMAKE_COMMAND} -E env PATH="${DEPOT_TOOLS_PATH}" ${DEPOT_TOOLS_PATH}/fetch.bat --no-history --force crashpad
    COMMAND ${CMAKE_COMMAND} -E env PATH="${PATH}" gclient sync
  CONFIGURE_COMMAND utils::gn gen out/Default
  BUILD_COMMAND ${CMAKE_COMMAND} -E env PATH="${PATH}" ${DEPOT_TOOLS_PATH}/ninja.exe -C out/Default
  INSTALL_COMMAND ${CMAKE_SCRIPT_PATH}/robocopy_flat.bat "<SOURCE_DIR>/out/Default/obj" "<INSTALL_DIR>/crashpad" "*.lib"
  INSTALL_DIR ${CRASHPAD_INSTALL_DIR}/lib
)
else()

set(SPEC "solutions = [\
  {\
    \"url\": \"https://chromium.googlesource.com/crashpad/crashpad.git\",\
    \"managed\": False,\
    \"name\": \"crashpad\",\
    \"custom_vars\": { \"pull_linux_clang\": True },\
  },\
]\
")

set(ARGS "clang_path=\"//third_party/linux/clang/linux-amd64\"\
  target_sysroot=\"//third_party/linux/sysroot\"")

ExternalProject_Add(
  crashpad
  BUILD_BYPRODUCTS ${_byproducts}
  BUILD_IN_SOURCE TRUE
  DEPENDS utils::gn utils::fetch utils::gclient utils::ninja
  STEP_TARGETS build install

  DOWNLOAD_COMMAND ${CMAKE_COMMAND} -E env PATH=${DEPOT_TOOLS_PATH}:$ENV{PATH} fetch --no-history --force crashpad
    COMMAND ${CMAKE_COMMAND} -E env PATH=${DEPOT_TOOLS_PATH}:$ENV{PATH} gclient config --spec ${SPEC}
    COMMAND ${CMAKE_COMMAND} -E env PATH=${DEPOT_TOOLS_PATH}:$ENV{PATH} gclient sync
  CONFIGURE_COMMAND utils::gn gen out/Default --args=${ARGS}
  BUILD_COMMAND ${CMAKE_COMMAND} -E env PATH=${DEPOT_TOOLS_PATH}:$ENV{PATH} CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} ninja -C out/Default
  INSTALL_COMMAND ""
  INSTALL_DIR ${CRASHPAD_INSTALL_DIR}/lib
)

ExternalProject_Add_Step(
  crashpad
  install-crashpad
  COMMAND find <SOURCE_DIR>/out -type f -name "*.a" -print0 | xargs -0 -I{} cp {} <INSTALL_DIR>
  DEPENDEES build
  DEPENDERS install
)
endif()

ExternalProject_Get_Property(
  crashpad
  source_dir
  install_dir
)

if(WIN32)
  set(CRASHPAD_LIBRARIES "${install_dir}/base.lib" "${install_dir}/client.lib" "${install_dir}/util.lib")
else()
  set(CRASHPAD_LIBRARIES "${install_dir}/libbase.a" "${install_dir}/libclient.a" "${install_dir}/libutil.a")
endif()

if(NOT TARGET utils::crashpad)
  add_library(utils::crashpad INTERFACE IMPORTED)
  add_dependencies(utils::crashpad crashpad-build crashpad-install)
  set_target_properties(utils::crashpad PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${source_dir}")
  set_target_properties(utils::crashpad PROPERTIES INTERFACE_LINK_DIRECTORIES "${install_dir}")
  set_target_properties(utils::crashpad PROPERTIES INTERFACE_LINK_LIBRARIES "${CRASHPAD_LIBRARIES}")
endif()

mark_as_advanced( CRASHPAD_LIBRARIES )