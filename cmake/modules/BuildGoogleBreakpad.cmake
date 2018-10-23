set(BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

if(WIN32 AND NOT MINGW)  
  set(_byproducts)
  if(NOT MSBUILD)
    set(_byproducts
      ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib/breakpad/client/windows/lib/common.lib
      ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib/breakpad/client/windows/handler/lib/exception_handler.lib
      ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib/breakpad/client/windows/crash_generation/lib/crash_generation_client.lib
      
      ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib/breakpad/client/windows/${BUILD_TYPE}/lib/common.lib
      ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib/breakpad/client/windows/handler/${BUILD_TYPE}/lib/exception_handler.lib
      ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib/breakpad/client/windows/crash_generation/${BUILD_TYPE}/lib/crash_generation_client.lib
    )
  endif()

  ExternalProject_Add(
    breakpad
    GIT_REPOSITORY https://chromium.googlesource.com/breakpad/breakpad
    GIT_TAG origin/chrome_64
    GIT_SHALLOW true
    INSTALL_DIR ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}
    STEP_TARGETS build
    BUILD_BYPRODUCTS ${_byproducts}

    UPDATE_COMMAND ""
    CONFIGURE_COMMAND <SOURCE_DIR>/src/tools/gyp/gyp.bat --no-circular-check <SOURCE_DIR>/src/client/windows/breakpad_client.gyp
    BUILD_COMMAND msbuild <SOURCE_DIR>/src/client/windows/handler/exception_handler.vcxproj /nologo /t:rebuild /m:2 /p:Configuration=${BUILD_TYPE} /p:Platform=${BUILD_PLATFORM}
    COMMAND ${CMAKE_SCRIPT_PATH}/robocopy.bat "<SOURCE_DIR>/src/" "<INSTALL_DIR>/lib/breakpad" "*.lib"
    COMMAND ${CMAKE_SCRIPT_PATH}/robocopy.bat "<SOURCE_DIR>/src/" "<INSTALL_DIR>/bin" "*.dll"
    INSTALL_COMMAND ""
  )

  ExternalProject_Add_Step(
    breakpad
    build-crash-generation-client
    DEPENDERS build
    COMMAND msbuild <SOURCE_DIR>/src/client/windows/crash_generation/crash_generation_client.vcxproj /nologo /t:rebuild /m:2 /p:Configuration=${BUILD_TYPE} /p:Platform=${BUILD_PLATFORM}
  )

  ExternalProject_Add_Step(
    breakpad
    build-common
    DEPENDERS build
    COMMAND msbuild <SOURCE_DIR>/src/client/windows/common.vcxproj /nologo /t:rebuild /m:2 /p:Configuration=${BUILD_TYPE} /p:Platform=${BUILD_PLATFORM}
  )

  ExternalProject_Add_Step(
    breakpad
    download-gyp
    DEPENDEES download
    DEPENDERS configure
    COMMAND ${CMAKE_COMMAND} -E remove_directory <SOURCE_DIR>/src/tools/gyp
    COMMAND git clone https://github.com/bnoordhuis/gyp.git <SOURCE_DIR>/src/tools/gyp
  )

  # Breakpad builds with /MT by default but we need /MD. This patch makes it build with /MD
  ExternalProject_Add_Step(
    breakpad
    patch_project_files
    DEPENDEES configure
    DEPENDERS build build-common build-crash-generation-client
    WORKING_DIRECTORY <SOURCE_DIR>
    COMMAND cmake -DVCXPROJ_PATH=<SOURCE_DIR>/src/client/windows/common.vcxproj -P ${CMAKE_SCRIPT_PATH}/breakpad_VS_patch.cmake
    COMMAND cmake -DVCXPROJ_PATH=<SOURCE_DIR>/src/client/windows/handler/exception_handler.vcxproj -P ${CMAKE_SCRIPT_PATH}/breakpad_VS_patch.cmake
    COMMAND cmake -DVCXPROJ_PATH=<SOURCE_DIR>/src/client/windows/crash_generation/crash_generation_client.vcxproj -P ${CMAKE_SCRIPT_PATH}/breakpad_VS_patch.cmake
  )
  
  ExternalProject_Add_Step(
    breakpad
    copy-breakpad
    DEPENDEES download
    DEPENDERS patch_project_files
    COMMAND ${CMAKE_SCRIPT_PATH}/robocopy.bat "<SOURCE_DIR>/src/" "<INSTALL_DIR>/include/breakpad" "*.h"
    COMMAND ${CMAKE_SCRIPT_PATH}/robocopy.bat "<SOURCE_DIR>/src/" "<INSTALL_DIR>/include/breakpad" "*.hpp"
    COMMAND ${CMAKE_SCRIPT_PATH}/robocopy.bat "<SOURCE_DIR>/src/tools/windows/binaries/" "<INSTALL_DIR>/bin" "*.exe"
  )
else()

  # This is a workaround for Ninja not allowing us to build if these libs weren't built before
  set(_byproducts
    ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib/libbreakpad.a
    ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib/libbreakpad_client.a
  )

  ExternalProject_Add(
    breakpad
    GIT_REPOSITORY https://chromium.googlesource.com/breakpad/breakpad
    GIT_TAG origin/chrome_64
    GIT_SHALLOW true
    INSTALL_DIR ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}
    STEP_TARGETS build
    
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR} --quiet --config-cache
    BUILD_BYPRODUCTS ${_byproducts}
  )

  ExternalProject_Add_Step(
    breakpad
    download-lss
    COMMAND ${CMAKE_COMMAND} -E remove_directory <SOURCE_DIR>/src/third_party/lss
    COMMAND git clone -q https://chromium.googlesource.com/linux-syscall-support <SOURCE_DIR>/src/third_party/lss
    DEPENDEES download
    DEPENDERS configure
  )
endif()

ExternalProject_Get_Property(
  breakpad
  source_dir
)

if(WIN32 AND NOT MINGW)
  set(BREAKPAD_DUMP_SYMS_EXEC ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/bin/dump_syms.exe)
  set(BREAKPAD_EXCEPTION_HANDLER_INCLUDE_DIR ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/include/breakpad)

  set(BREAKPAD_COMMON_LIBRARY_DIR "${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib/breakpad/client/windows/${BUILD_TYPE}/lib")
  set(BREAKPAD_CRASH_CLIENT_LIBRARY_DIR "${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib/breakpad/client/windows/crash_generation/${BUILD_TYPE}/lib")
  set(BREAKPAD_EXCEPTION_HANDLER_LIBRARY_DIR "${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib/breakpad/client/windows/handler/${BUILD_TYPE}/lib")
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

if(NOT TARGET Breakpad::Breakpad)
  add_library(Breakpad::Breakpad INTERFACE IMPORTED)
  add_dependencies(Breakpad::Breakpad breakpad-build)
  set_target_properties(Breakpad::Breakpad PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${BREAKPAD_EXCEPTION_HANDLER_INCLUDE_DIR}")
  set_target_properties(Breakpad::Breakpad PROPERTIES INTERFACE_LINK_LIBRARIES "${BREAKPAD_EXCEPTION_HANDLER_LIBRARIES}")
endif()