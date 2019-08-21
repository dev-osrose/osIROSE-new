# -*- cmake -*-

# - Find Google BreakPad
# Find the Google BreakPad includes and library
# This module defines
#  CRASHPAD_EXCEPTION_HANDLER_INCLUDE_DIR, where to find exception_handler.h, etc.
#  CRASHPAD_EXCEPTION_HANDLER_LIBRARIES, the libraries needed to use Google BreakPad.
#  CRASHPAD_EXCEPTION_HANDLER_FOUND, If false, do not try to use Google BreakPad.
# also defined, but not for general use are
#  CRASHPAD_EXCEPTION_HANDLER_LIBRARY, where to find the Google BreakPad library.

find_path(CRASHPAD_INCLUDE_DIR
        client/crashpad_client.h
        HINTS
            ${CMAKE_BINARY_DIR}/crashpad-prefix/src/crashpad
            ${CMAKE_THIRD_PARTY_DIR}/include/crashpad)

find_path(CRASHPAD_MINI_CHROMIUM_INCLUDE_DIR
        base/files/file_path.h
        HINTS
            ${CMAKE_BINARY_DIR}/crashpad-prefix/src/crashpad
            ${CMAKE_BINARY_DIR}/crashpad-prefix/src/crashpad/third_party
            ${CMAKE_BINARY_DIR}/crashpad-prefix/src/crashpad/third_party/mini_chromium
            ${CMAKE_BINARY_DIR}/crashpad-prefix/src/crashpad/third_party/mini_chromium/mini_chromium
            ${CMAKE_THIRD_PARTY_DIR}/include/crashpad
            ${CMAKE_THIRD_PARTY_DIR}/include/crashpad/third_party
            ${CMAKE_THIRD_PARTY_DIR}/include/crashpad/third_party/mini_chromium
            ${CMAKE_THIRD_PARTY_DIR}/include/crashpad/third_party/mini_chromium/mini_chromium)

if(WIN32)
    set(CRASHPAD_BASE_LIB_NAMES base)
    set(CRASHPAD_CLIENT_NAMES client)
    set(CRASHPAD_UTILS_NAMES util)
else()
    set(CRASHPAD_BASE_LIB_NAMES libbase.a)
    set(CRASHPAD_CLIENT_NAMES libclient.a)
    set(CRASHPAD_UTILS_NAMES libutil.a)
endif()

find_library(CRASHPAD_CLIENT_LIBRARY
  NAMES ${CRASHPAD_CLIENT_NAMES}
  HINTS ${CMAKE_BINARY_DIR}/crashpad-prefix/src/crashpad/out
        ${CMAKE_THIRD_PARTY_DIR}/lib
)
find_library(CRASHPAD_BASE_LIBRARY
  NAMES ${CRASHPAD_BASE_LIB_NAMES}
  HINTS ${CMAKE_BINARY_DIR}/crashpad-prefix/src/crashpad/out
        ${CMAKE_THIRD_PARTY_DIR}/lib)

find_library(CRASHPAD_UTILS_LIBRARY
  NAMES ${CRASHPAD_UTILS_NAMES}
  HINTS ${CMAKE_BINARY_DIR}/crashpad-prefix/src/crashpad/out
        ${CMAKE_THIRD_PARTY_DIR}/lib)

if (CRASHPAD_CLIENT_LIBRARY AND CRASHPAD_INCLUDE_DIR)
    find_path(CRASHPAD_LIB_DIR
      NAMES ${CRASHPAD_CLIENT_NAMES}
      HINTS ${CMAKE_BINARY_DIR}/crashpad-prefix/src/crashpad/out
            ${CMAKE_THIRD_PARTY_DIR}/lib)
endif ()


include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set CRASHPAD_FOUND to TRUE if
# all listed variables are TRUE
find_package_handle_standard_args(crashpad
                                  REQUIRED_VARS CRASHPAD_BASE_LIBRARY CRASHPAD_CLIENT_LIBRARY CRASHPAD_UTILS_LIBRARY CRASHPAD_INCLUDE_DIR CRASHPAD_MINI_CHROMIUM_INCLUDE_DIR)

mark_as_advanced(
  CRASHPAD_BASE_LIBRARY
  CRASHPAD_CLIENT_LIBRARY
  CRASHPAD_UTILS_LIBRARY
  CRASHPAD_INCLUDE_DIR
  CRASHPAD_MINI_CHROMIUM_INCLUDE_DIR
)

if(CRASHPAD_FOUND AND NOT TARGET utils::crashpad)
  set(CRASHPAD_LIBRARIES ${CRASHPAD_CLIENT_LIBRARY} ${CRASHPAD_UTILS_LIBRARY} ${CRASHPAD_BASE_LIBRARY})

  add_library(utils::crashpad INTERFACE IMPORTED)
  target_include_directories(utils::crashpad SYSTEM INTERFACE "${CRASHPAD_INCLUDE_DIR}")
  target_include_directories(utils::crashpad SYSTEM INTERFACE "${CRASHPAD_MINI_CHROMIUM_INCLUDE_DIR}")
  target_link_directories(utils::crashpad INTERFACE "${CRASHPAD_LIB_DIR}")
  target_link_libraries(utils::crashpad INTERFACE "${CRASHPAD_LIBRARIES}")
endif()
