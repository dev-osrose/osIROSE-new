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

ExternalProject_Add(
  crashpad
  GIT_REPOSITORY https://chromium.googlesource.com/crashpad/crashpad
  BUILD_BYPRODUCTS ${_byproducts}
  BUILD_IN_SOURCE TRUE
  DEPENDS utils::gn
  STEP_TARGETS build install

  CONFIGURE_COMMAND utils::gn gen out/Default
  BUILD_COMMAND CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} ninja -C out/Default
  INSTALL_COMMAND ""
  INSTALL_DIR ${CRASHPAD_INSTALL_DIR}/lib
)

#ExternalProject_Add_Step(
#  crashpad
#  download-clang
#  COMMAND ${CMAKE_COMMAND} -E remove_directory <SOURCE_DIR>/third_party/linux/clang
#  COMMAND git clone -q https://chromium.googlesource.com/chromium/mini_chromium <SOURCE_DIR>/third_party/linux/clang
#  DEPENDEES download
#  DEPENDERS configure build
#)

ExternalProject_Add_Step(
  crashpad
  download-mini_chromium
  COMMAND ${CMAKE_COMMAND} -E remove_directory <SOURCE_DIR>/third_party/mini_chromium/mini_chromium
  COMMAND git clone -q https://chromium.googlesource.com/chromium/mini_chromium <SOURCE_DIR>/third_party/mini_chromium/mini_chromium
  DEPENDEES download
  DEPENDERS configure build
)

ExternalProject_Add_Step(
  crashpad
  download-gtest
  COMMAND ${CMAKE_COMMAND} -E remove_directory <SOURCE_DIR>/third_party/gtest/gtest
  COMMAND git clone -q https://github.com/google/googletest.git <SOURCE_DIR>/third_party/gtest/gtest
  DEPENDEES download
  DEPENDERS configure build
)

ExternalProject_Add_Step(
  crashpad
  download-gyp
  COMMAND ${CMAKE_COMMAND} -E remove_directory <SOURCE_DIR>/third_party/gyp/gyp
  COMMAND git clone -q https://github.com/bnoordhuis/gyp.git <SOURCE_DIR>/third_party/gyp/gyp
  DEPENDEES download
  DEPENDERS configure build
)

if(NOT WIN32)
  ExternalProject_Add_Step(
    crashpad
    download-lss
    COMMAND ${CMAKE_COMMAND} -E remove_directory <SOURCE_DIR>/third_party/lss/lss
    COMMAND git clone -q https://chromium.googlesource.com/linux-syscall-support <SOURCE_DIR>/third_party/lss/lss
    DEPENDEES download
    DEPENDERS configure build
  )
  
  ExternalProject_Add_Step(
    crashpad
    install-crashpad
    COMMAND find <SOURCE_DIR> -type f -name "*.a" -print0 | xargs -0 -I{} cp {} <INSTALL_DIR>
    DEPENDEES build
    DEPENDERS install
  )
else()
  ExternalProject_Add_Step(
    crashpad
    install-crashpad
    COMMAND ${CMAKE_SCRIPT_PATH}/robocopy.bat "<SOURCE_DIR>" "<INSTALL_DIR>" "*.dll"
    COMMAND ${CMAKE_SCRIPT_PATH}/robocopy.bat "<SOURCE_DIR>" "<INSTALL_DIR>" "*.lib"
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
  set(CRASHPAD_LIBRARIES "base.lib" "client.lib" "util.lib")
else()
  set(CRASHPAD_LIBRARIES "libbase.a" "libclient.a" "libutil.a")
endif()

if(NOT TARGET utils::crashpad)
  add_library(utils::crashpad INTERFACE IMPORTED)
  add_dependencies(utils::crashpad crashpad-build crashpad-install)
  set_target_properties(utils::crashpad PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${source_dir}")
  set_target_properties(utils::crashpad PROPERTIES INTERFACE_LINK_DIRECTORIES "${install_dir}")
  set_target_properties(utils::crashpad PROPERTIES INTERFACE_LINK_LIBRARIES "${CRASHPAD_LIBRARIES}")
endif()
