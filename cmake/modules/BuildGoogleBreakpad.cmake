set(BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

set(_byproducts
    ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib/libbreakpad.a
    ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib/libbreakpad_client.a
    ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib/breakpad/client/windows/lib/common.lib
    ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib/breakpad/client/windows/handler/lib/exception_handler.lib
    ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib/breakpad/client/windows/crash_generation/lib/crash_generation_client.lib

    ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib/breakpad/client/windows/${CMAKE_BUILD_TYPE}/lib/common.lib
    ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib/breakpad/client/windows/handler/${CMAKE_BUILD_TYPE}/lib/exception_handler.lib
    ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib/breakpad/client/windows/crash_generation/${CMAKE_BUILD_TYPE}/lib/crash_generation_client.lib
  )

if(WIN32)
  ExternalProject_Add(
    breakpad
    GIT_REPOSITORY https://chromium.googlesource.com/breakpad/breakpad
    GIT_TAG origin/chrome_64
    GIT_SHALLOW true
    INSTALL_DIR ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}
    STEP_TARGETS build install
    BUILD_BYPRODUCTS ${_byproducts}

    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
#    CONFIGURE_COMMAND Python2::Interpreter <SOURCE_DIR>/src/tools/gyp/gyp_main.py --no-circular-check <SOURCE_DIR>/src/client/windows/breakpad_client.gyp
#      COMMAND cmake -DVCXPROJ_PATH=<SOURCE_DIR>/src/client/windows/common.vcxproj -P ${CMAKE_SCRIPT_PATH}/breakpad_VS_patch.cmake
#      COMMAND cmake -DVCXPROJ_PATH=<SOURCE_DIR>/src/client/windows/handler/exception_handler.vcxproj -P ${CMAKE_SCRIPT_PATH}/breakpad_VS_patch.cmake
#      COMMAND cmake -DVCXPROJ_PATH=<SOURCE_DIR>/src/client/windows/crash_generation/crash_generation_client.vcxproj -P ${CMAKE_SCRIPT_PATH}/breakpad_VS_patch.cmake
#
#    BUILD_COMMAND msbuild <SOURCE_DIR>/src/client/windows/common.vcxproj /nologo /t:rebuild /m:2 /p:Configuration=$<CONFIG> /p:Platform=${BUILD_PLATFORM}
#      COMMAND msbuild <SOURCE_DIR>/src/client/windows/crash_generation/crash_generation_client.vcxproj /nologo /t:rebuild /m:2 /p:Configuration=$<CONFIG> /p:Platform=${BUILD_PLATFORM}
#      COMMAND msbuild <SOURCE_DIR>/src/client/windows/handler/exception_handler.vcxproj /nologo /t:rebuild /m:2 /p:Configuration=$<CONFIG> /p:Platform=${BUILD_PLATFORM}

    INSTALL_COMMAND ${CMAKE_SCRIPT_PATH}/robocopy.bat "<SOURCE_DIR>/src/" "<INSTALL_DIR>/lib/breakpad" "*.lib"
      COMMAND ${CMAKE_SCRIPT_PATH}/robocopy.bat "<SOURCE_DIR>/src/" "<INSTALL_DIR>/bin" "*.dll"
      COMMAND ${CMAKE_SCRIPT_PATH}/robocopy.bat "<SOURCE_DIR>/src/" "<INSTALL_DIR>/include/breakpad" "*.h"
      COMMAND ${CMAKE_SCRIPT_PATH}/robocopy.bat "<SOURCE_DIR>/src/" "<INSTALL_DIR>/include/breakpad" "*.hpp"
      COMMAND ${CMAKE_SCRIPT_PATH}/robocopy.bat "<SOURCE_DIR>/src/tools/windows/binaries/" "<INSTALL_DIR>/bin" "*.exe"
  )

  ExternalProject_Add_Step(
    breakpad
    download-gyp
    DEPENDEES download
    DEPENDERS configure
    COMMAND ${CMAKE_COMMAND} -E remove_directory <SOURCE_DIR>/src/tools/gyp
    COMMAND git clone https://github.com/bnoordhuis/gyp.git <SOURCE_DIR>/src/tools/gyp
  )
else()
  ExternalProject_Add(
    breakpad
    GIT_REPOSITORY https://chromium.googlesource.com/breakpad/breakpad
    GIT_TAG origin/chrome_64
    GIT_SHALLOW true
    INSTALL_DIR ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}
    STEP_TARGETS build install
    UPDATE_COMMAND ""
    #PATCH_COMMAND ${PATCH_SCRIPT_PATH} ${CMAKE_PATCH_DIR}/breakpad_upload.patch
    CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR} --quiet --config-cache
    BUILD_BYPRODUCTS ${_byproducts}
  )

  ExternalProject_Add_Step(
    breakpad
    download-lss
    COMMAND ${CMAKE_COMMAND} -E remove_directory <SOURCE_DIR>/src/third_party/lss
    COMMAND git clone -q https://chromium.googlesource.com/linux-syscall-support <SOURCE_DIR>/src/third_party/lss
    DEPENDEES download
    DEPENDERS configure build
  )
endif()

ExternalProject_Get_Property(
  breakpad
  source_dir
)

if(WIN32 AND NOT MINGW)
  set(BREAKPAD_DUMP_SYMS_EXEC ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/bin/dump_syms.exe)
  set(BREAKPAD_EXCEPTION_HANDLER_INCLUDE_DIR ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/include/breakpad)

  set(BREAKPAD_COMMON_LIBRARY_DIR "${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib/breakpad/client/windows/${CMAKE_BUILD_TYPE}/lib")
  set(BREAKPAD_CRASH_CLIENT_LIBRARY_DIR "${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib/breakpad/client/windows/crash_generation/${CMAKE_BUILD_TYPE}/lib")
  set(BREAKPAD_EXCEPTION_HANDLER_LIBRARY_DIR "${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib/breakpad/client/windows/handler/${CMAKE_BUILD_TYPE}/lib")
  set(BREAKPAD_EXCEPTION_HANDLER_LIBRARIES
    "${BREAKPAD_COMMON_LIBRARY_DIR}/common.lib"
    "${BREAKPAD_EXCEPTION_HANDLER_LIBRARY_DIR}/exception_handler.lib"
    "${BREAKPAD_CRASH_CLIENT_LIBRARY_DIR}/crash_generation_client.lib"
  )
else()
  set(BREAKPAD_DUMP_SYMS_EXEC ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/bin/dump_syms)
  set(BREAKPAD_EXCEPTION_HANDLER_INCLUDE_DIR ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/include/breakpad)

  set(BREAKPAD_EXCEPTION_HANDLER_LIBRARY_DIR ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib)
  set(BREAKPAD_EXCEPTION_HANDLER_LIBRARIES "${BREAKPAD_EXCEPTION_HANDLER_LIBRARY_DIR}/libbreakpad.a")
  if(NOT MINGW)
    set(BREAKPAD_EXCEPTION_HANDLER_LIBRARIES ${BREAKPAD_EXCEPTION_HANDLER_LIBRARIES} "${BREAKPAD_EXCEPTION_HANDLER_LIBRARY_DIR}/libbreakpad_client.a")
  endif()
endif()

file(MAKE_DIRECTORY ${source_dir}/src)
file(MAKE_DIRECTORY ${BREAKPAD_EXCEPTION_HANDLER_INCLUDE_DIR})

if(NOT TARGET utils::breakpad)
  add_library(utils::breakpad INTERFACE IMPORTED)
  add_dependencies(utils::breakpad breakpad-build breakpad-install)
  set_target_properties(utils::breakpad PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${BREAKPAD_EXCEPTION_HANDLER_INCLUDE_DIR}")
#  set_target_properties(utils::breakpad PROPERTIES INTERFACE_LINK_LIBRARIES "${BREAKPAD_EXCEPTION_HANDLER_LIBRARIES}")
endif()

if(NOT TARGET utils::dump_syms)
  add_executable(utils::dump_syms IMPORTED IMPORTED)
  add_dependencies(utils::dump_syms breakpad-install)
  set_target_properties(utils::dump_syms PROPERTIES IMPORTED_LOCATION ${BREAKPAD_DUMP_SYMS_EXEC})
endif()

