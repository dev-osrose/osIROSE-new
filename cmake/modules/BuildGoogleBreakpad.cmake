set(BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR ${CMAKE_EXTERNAL_BINARY_DIR})

if(WIN32 AND NOT MINGW)
  if(DEBUG OR Debug)
    set(CONFIGURATION_TYPE Debug)
  else()
    set(CONFIGURATION_TYPE Release)
  endif()
  
  ExternalProject_Add(
    breakpad
    GIT_SUBMODULES breakpad
    SOURCE_DIR ${CMAKE_THIRD_PARTY_DIR}/breakpad
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ${CMAKE_THIRD_PARTY_DIR}/breakpad/src/tools/gyp/gyp.bat --no-circular-check ${CMAKE_THIRD_PARTY_DIR}/breakpad/src/client/windows/breakpad_client.gyp
    BUILD_COMMAND msbuild <SOURCE_DIR>/src/client/windows/handler/exception_handler.vcxproj /nologo /t:rebuild /m:2 /property:Configuration=${CONFIGURATION_TYPE} /property:WindowsTargetPlatformVersion=10.0.14393.0
    INSTALL_COMMAND ""
  )
  
  #TODO figure out how to build this without the "/property:WindowsTargetPlatformVersion=10.0.14393.0" setting
  
  ExternalProject_Add_Step(
    breakpad
    download-gyp
    DEPENDEES download
	  DEPENDERS configure
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_THIRD_PARTY_DIR}/breakpad/src/tools/gyp
    COMMAND git clone https://github.com/bnoordhuis/gyp.git ${CMAKE_THIRD_PARTY_DIR}/breakpad/src/tools/gyp
  )
  
  # Breakpad builds with /MT by default but we need /MD. This patch makes it build with /MD
  ExternalProject_Add_Step(
    breakpad
    patch_project_files
    DEPENDEES configure
    DEPENDERS build
    WORKING_DIRECTORY <SOURCE_DIR>
    COMMAND cmake -DVCXPROJ_PATH=<SOURCE_DIR>/src/client/windows/handler/exception_handler.vcxproj -P ${CMAKE_SCRIPT_PATH}/breakpad_VS_patch.cmake
  )
else()

  # This is a workaround for Ninja not allowing us to build if these libs weren't built before
  set(_byproducts
    ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib/libbreakpad.a
    ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib/libbreakpad_client.a
  )
  
  ExternalProject_Add(
    breakpad
    GIT_SUBMODULES breakpad
    SOURCE_DIR ${CMAKE_THIRD_PARTY_DIR}/breakpad
    BUILD_BYPRODUCTS ${_byproducts}
    CONFIGURE_COMMAND sh <SOURCE_DIR>/configure --prefix=${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}
  )
  
  ExternalProject_Add_Step(
    breakpad
    download-lss
    COMMAND ${CMAKE_COMMAND} -E remove_directory src/third_party/lss
    COMMAND git clone -q https://chromium.googlesource.com/linux-syscall-support src/third_party/lss
    WORKING_DIRECTORY <SOURCE_DIR>
    DEPENDEES download
    DEPENDERS configure
  )
endif()

ExternalProject_Get_Property(
  breakpad
  source_dir
)
set(BREAKPAD_EXCEPTION_HANDLER_INCLUDE_DIR ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/include/breakpad)

if(WIN32 AND NOT MINGW)
  set(BREAKPAD_EXCEPTION_HANDLER_LIBRARY_DIR ${source_dir}/src/client/windows/handler/${CONFIGURATION_TYPE}/lib)
  set(BREAKPAD_EXCEPTION_HANDLER_LIBRARIES "${BREAKPAD_EXCEPTION_HANDLER_LIBRARY_DIR}/exception_handler.lib")
else()
  set(BREAKPAD_EXCEPTION_HANDLER_LIBRARIES "libbreakpad.a")
  if(NOT MINGW)
    set(BREAKPAD_EXCEPTION_HANDLER_LIBRARIES "${BREAKPAD_EXCEPTION_HANDLER_LIBRARIES};libbreakpad_client.a")
  endif()
endif()

SET_PROPERTY(TARGET breakpad                PROPERTY FOLDER "ThirdParty")
