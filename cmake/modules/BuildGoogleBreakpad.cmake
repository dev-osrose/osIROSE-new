set(BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR ${CMAKE_EXTERNAL_BINARY_DIR}/breakpad)

if(WIN32 AND NOT MINGW)
  if(DEBUG)
    set(CONFIGURATION_TYPE Debug)
  else()
    set(CONFIGURATION_TYPE Release)
  endif()

## TODO - setup MSVC PROJECT

else()

  ExternalProject_Add(
    breakpad
    GIT_SUBMODULES breakpad
    SOURCE_DIR ${CMAKE_THIRD_PARTY_DIR}/breakpad
    CONFIGURE_COMMAND sh <SOURCE_DIR>/configure --enable-shared=no --enable-static=yes --prefix=${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}
  )
  
#  ExternalProject_Add_Step(
#    breakpad
#    patch-bug-158
#    COMMAND ${PATCH_SCRIPT_PATH} ${CMAKE_PATCH_DIR}/bug-158.patch
#    WORKING_DIRECTORY <SOURCE_DIR>
#    DEPENDEES download
#    DEPENDERS configure
#  )
#  if(MINGW)
#    ExternalProject_Add_Step(
#      breakpad
#      patch-mingw
#      COMMAND ${PATCH_SCRIPT_PATH} ${CMAKE_PATCH_DIR}/breakpad-mingw.patch
#      WORKING_DIRECTORY <SOURCE_DIR>
#      DEPENDEES download
#      DEPENDERS configure
#    )
#    ExternalProject_Add_Step(
#      breakpad
#      patch-windows
#      COMMAND ${PATCH_SCRIPT_PATH} ${CMAKE_PATCH_DIR}/breakpad.patch
#      WORKING_DIRECTORY <SOURCE_DIR>
#      DEPENDEES download
#      DEPENDERS configure
#    )
#  endif()
endif()

ExternalProject_Get_Property(
  breakpad
  source_dir
)
set(BREAKPAD_EXCEPTION_HANDLER_INCLUDE_DIR ${source_dir}/src)

if(WIN32 AND NOT MINGW)
  set(BREAKPAD_EXCEPTION_HANDLER_LIBRARY_DIR ${source_dir}/src/client/windows/handler/${CONFIGURATION_TYPE}/lib)
  set(BREAKPAD_EXCEPTION_HANDLER_LIBRARIES "${BREAKPAD_EXCEPTION_HANDLER_LIBRARY_DIR}/exception_handler.lib")
else()
  set(BREAKPAD_EXCEPTION_HANDLER_LIBRARY_DIR ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/lib)
  set(BREAKPAD_EXCEPTION_HANDLER_LIBRARIES "${BREAKPAD_EXCEPTION_HANDLER_LIBRARY_DIR}/libbreakpad.a")
  if(NOT MINGW)
    set(BREAKPAD_EXCEPTION_HANDLER_LIBRARIES "${BREAKPAD_EXCEPTION_HANDLER_LIBRARIES};${BREAKPAD_EXCEPTION_HANDLER_LIBRARY_DIR}/libbreakpad_client.a")
  endif()
endif()

SET_PROPERTY(TARGET breakpad                PROPERTY FOLDER "ThirdParty")