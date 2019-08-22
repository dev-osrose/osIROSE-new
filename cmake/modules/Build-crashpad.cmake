set(CRASHPAD_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

set(_byproducts
  ${CRASHPAD_INSTALL_DIR}/lib/libbase.a
  ${CRASHPAD_INSTALL_DIR}/lib/libutil.a
  ${CRASHPAD_INSTALL_DIR}/lib/libclient.a
  ${CRASHPAD_INSTALL_DIR}/lib/libhandler.a
  ${CRASHPAD_INSTALL_DIR}/lib/libminidump.a
  ${CRASHPAD_INSTALL_DIR}/lib/base.lib
  ${CRASHPAD_INSTALL_DIR}/lib/util.lib
  ${CRASHPAD_INSTALL_DIR}/lib/client.lib
  ${CRASHPAD_INSTALL_DIR}/lib/handler.lib
  ${CRASHPAD_INSTALL_DIR}/lib/minidump.lib
)

if(WIN32)
file(TO_CMAKE_PATH "$ENV{PATH}" ENV_PATH_VAR)
message(STATUS ${ENV_PATH_VAR})

ExternalProject_Add(
  crashpad
  BUILD_BYPRODUCTS ${_byproducts}
  BUILD_IN_SOURCE TRUE
  DEPENDS utils::gn utils::fetch utils::gclient utils::ninja
  STEP_TARGETS build install

  DOWNLOAD_COMMAND ${CMAKE_COMMAND} -E env PATH="${ENV_PATH_VAR}" ${DEPOT_TOOLS_PATH}/fetch.bat --no-history --force crashpad
    COMMAND ${CMAKE_COMMAND} -E env PATH="${ENV_PATH_VAR}" gclient sync
  CONFIGURE_COMMAND utils::gn gen out/Default
  BUILD_COMMAND ${CMAKE_COMMAND} -E env PATH="${ENV_PATH_VAR}" ${DEPOT_TOOLS_PATH}/ninja.exe -C out/Default
  INSTALL_COMMAND ${CMAKE_SCRIPT_PATH}/robocopy_flat.bat "<SOURCE_DIR>/out/Default/obj" "<INSTALL_DIR>/lib/crashpad" "*.lib"
    COMMAND ${CMAKE_SCRIPT_PATH}/robocopy.bat "<SOURCE_DIR>" "${CRASHPAD_INSTALL_DIR}/include" "*.h"
    COMMAND ${CMAKE_SCRIPT_PATH}/robocopy_flat.bat "<SOURCE_DIR>/out/Default" "<INSTALL_DIR>/bin" "*.exe"
  INSTALL_DIR ${CRASHPAD_INSTALL_DIR}
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
  INSTALL_DIR ${CRASHPAD_INSTALL_DIR}
)

ExternalProject_Add_Step(
  crashpad
  install-crashpad
  WORKING_DIRECTORY <SOURCE_DIR>
  COMMAND find <SOURCE_DIR>/out -type f -name "*.a" -print0 | xargs -0 -I{} cp {} <INSTALL_DIR>/lib
  COMMAND find . -type f -name "*.h" -print0 | xargs -0 -I{} cp --parents {} <INSTALL_DIR>/include/crashpad
  COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/out/Default/crashpad_handler <INSTALL_DIR>/bin/crashpad_handler
  COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/out/Default/crashpad_http_upload <INSTALL_DIR>/bin/crashpad_http_upload
  COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/out/Default/crashpad_handler ${CMAKE_BINARY_DIR}/bin/crashpad_handler
  COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/out/Default/crashpad_http_upload ${CMAKE_BINARY_DIR}/bin/crashpad_http_upload
  DEPENDEES build
  DEPENDERS install
)
endif()

ExternalProject_Get_Property(
  crashpad
  source_dir
  install_dir
)

file(MAKE_DIRECTORY ${install_dir}/include/crashpad/third_party/mini_chromium/mini_chromium)

if(WIN32)
  set(CRASHPAD_LIBRARIES "${install_dir}/lib/client.lib" "${install_dir}/lib/util.lib" "${install_dir}/lib/base.lib")
else()
  set(CRASHPAD_LIBRARIES "${install_dir}/lib/libclient.a" "${install_dir}/lib/libutil.a" "${install_dir}/lib/libbase.a")
endif()

if(NOT TARGET utils::crashpad)
  add_library(utils::crashpad INTERFACE IMPORTED)
  add_dependencies(utils::crashpad crashpad-build crashpad-install)
  target_include_directories(utils::crashpad SYSTEM INTERFACE "${install_dir}/include/crashpad")
  target_include_directories(utils::crashpad SYSTEM INTERFACE "${install_dir}/include/crashpad/third_party/mini_chromium/mini_chromium")
  target_link_directories(utils::crashpad INTERFACE "${install_dir}/lib")
  target_link_libraries(utils::crashpad INTERFACE "${CRASHPAD_LIBRARIES}")
endif()

mark_as_advanced( CRASHPAD_LIBRARIES )