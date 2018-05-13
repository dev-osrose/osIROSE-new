set(BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

if(WIN32 AND NOT MINGW)
  if(DEBUG OR Debug)
    set(CONFIGURATION_TYPE Debug)
  else()
    set(CONFIGURATION_TYPE Release)
  endif()
  
  ExternalProject_Add(
    breakpad
    GIT_REPOSITORY https://chromium.googlesource.com/breakpad/breakpad
    GIT_TAG origin/chrome_64
    GIT_SHALLOW true
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND <SOURCE_DIR>/src/tools/gyp/gyp.bat --no-circular-check <SOURCE_DIR>/src/client/windows/breakpad_client.gyp
    BUILD_COMMAND msbuild <SOURCE_DIR>/src/client/windows/handler/exception_handler.vcxproj /nologo /t:rebuild /m:2 /property:Configuration=$<CONFIG> /property:WindowsTargetPlatformVersion=10.0.14393.0
    INSTALL_COMMAND ""
  )
  
  ExternalProject_Add_Step(
    breakpad
    build-crash-generation-client
    COMMAND msbuild <SOURCE_DIR>/src/client/windows/crash_generation/crash_generation_client.vcxproj /nologo /t:rebuild /m:2 /property:Configuration=$<CONFIG> /property:WindowsTargetPlatformVersion=10.0.14393.0
  )
  
  ExternalProject_Add_Step(
    breakpad
    build-common
    COMMAND msbuild <SOURCE_DIR>/src/client/windows/common.vcxproj /nologo /t:rebuild /m:2 /property:Configuration=$<CONFIG> /property:WindowsTargetPlatformVersion=10.0.14393.0
  )
  
  #TODO figure out how to build this without the "/property:WindowsTargetPlatformVersion=10.0.14393.0" setting
  
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
    CONFIGURE_COMMAND sh <SOURCE_DIR>/configure --prefix=${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}
    BUILD_BYPRODUCTS ${_byproducts}
    INSTALL_DIR ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}
    #INSTALL_COMMAND mkdir -p <INSTALL_DIR>/include/breakpad && cp -fr <SOURCE_DIR>/src <INSTALL_DIR>/include/breakpad && cp -f <BINARY_DIR>/src/libbreakpad.a <BINARY_DIR>/src/client/linux/libbreakpad_client.a <INSTALL_DIR>/lib && cp -f <BINARY_DIR>/src/tools/linux/dump_syms/dump_syms <INSTALL_DIR>/bin
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
  set(BREAKPAD_EXCEPTION_HANDLER_INCLUDE_DIR ${source_dir}/src)
  set(BREAKPAD_COMMON_LIBRARY_DIR "${source_dir}/src/client/windows/$<CONFIG>/lib")
  set(BREAKPAD_CRASH_CLIENT_LIBRARY_DIR "${source_dir}/src/client/windows/crash_generation/$<CONFIG>/lib")
  set(BREAKPAD_EXCEPTION_HANDLER_LIBRARY_DIR "${source_dir}/src/client/windows/handler/$<CONFIG>/lib")
  set(BREAKPAD_EXCEPTION_HANDLER_LIBRARIES "${BREAKPAD_COMMON_LIBRARY_DIR}/common.lib;${BREAKPAD_EXCEPTION_HANDLER_LIBRARY_DIR}/exception_handler.lib;${BREAKPAD_CRASH_CLIENT_LIBRARY_DIR}/crash_generation_client.lib")
else()
  set(BREAKPAD_EXCEPTION_HANDLER_INCLUDE_DIR ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/include/breakpad)
  set(BREAKPAD_EXCEPTION_HANDLER_LIBRARY_DIR ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib)
  set(BREAKPAD_EXCEPTION_HANDLER_LIBRARIES "${BREAKPAD_EXCEPTION_HANDLER_LIBRARY_DIR}/libbreakpad.a")
  if(NOT MINGW)
    set(BREAKPAD_EXCEPTION_HANDLER_LIBRARIES "${BREAKPAD_EXCEPTION_HANDLER_LIBRARIES};${BREAKPAD_EXCEPTION_HANDLER_LIBRARY_DIR}/libbreakpad_client.a")
  endif()
endif()
