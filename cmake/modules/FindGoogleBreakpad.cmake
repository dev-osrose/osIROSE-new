# -*- cmake -*-

# - Find Google BreakPad
# Find the Google BreakPad includes and library
# This module defines
#  BREAKPAD_EXCEPTION_HANDLER_INCLUDE_DIR, where to find exception_handler.h, etc.
#  BREAKPAD_EXCEPTION_HANDLER_LIBRARIES, the libraries needed to use Google BreakPad.
#  BREAKPAD_EXCEPTION_HANDLER_FOUND, If false, do not try to use Google BreakPad.
# also defined, but not for general use are
#  BREAKPAD_EXCEPTION_HANDLER_LIBRARY, where to find the Google BreakPad library.

if(WIN32)
    find_path(BREAKPAD_EXCEPTION_HANDLER_INCLUDE_DIR 
        google_breakpad/common/breakpad_types.h
        HINTS
            ${CMAKE_THIRD_PARTY_DIR}/include/breakpad
            ${CMAKE_BINARY_DIR}/breakpad-prefix/src/breakpad/src)
    
    set(BREAKPAD_COMMON_LIB_NAMES common)
    set(BREAKPAD_EXCEPTION_HANDLER_NAMES exception_handler)
    set(BREAKPAD_CRASH_GENERATION_CLIENT_NAMES crash_generation_client)
else()
    find_path(BREAKPAD_EXCEPTION_HANDLER_INCLUDE_DIR 
        google_breakpad/common/breakpad_types.h
        HINTS
            ${CMAKE_THIRD_PARTY_DIR}/include/breakpad)
            
    find_program(BREAKPAD_DUMP_SYMS_PATH 
        dump_syms
        HINTS
            ${CMAKE_THIRD_PARTY_DIR}/bin)
            
    set(BREAKPAD_EXCEPTION_HANDLER_NAMES ${BREAKPAD_EXCEPTION_HANDLER_NAMES} breakpad_client)
endif()

find_library(BREAKPAD_EXCEPTION_HANDLER_LIBRARY
  NAMES ${BREAKPAD_EXCEPTION_HANDLER_NAMES}
  HINTS ${CMAKE_THIRD_PARTY_DIR}/lib
  )
  
if(WIN32)
    find_library(BREAKPAD_COMMON_LIBRARY
      NAMES ${BREAKPAD_COMMON_LIB_NAMES}
      HINTS ${CMAKE_THIRD_PARTY_DIR}/lib
      )
      
    find_library(BREAKPAD_CRASH_GEN_LIBRARY
      NAMES ${BREAKPAD_CRASH_GENERATION_CLIENT_NAMES}
      HINTS ${CMAKE_THIRD_PARTY_DIR}/lib
      )
endif()

if (BREAKPAD_EXCEPTION_HANDLER_LIBRARY AND BREAKPAD_EXCEPTION_HANDLER_INCLUDE_DIR)
    set(BREAKPAD_EXCEPTION_HANDLER_LIBRARIES ${BREAKPAD_EXCEPTION_HANDLER_LIBRARY})
    
    if(WIN32)
        if(BREAKPAD_COMMON_LIBRARY AND BREAKPAD_CRASH_GEN_LIBRARY)
            set(BREAKPAD_EXCEPTION_HANDLER_FOUND "YES")
            set(BREAKPAD_EXCEPTION_HANDLER_LIBRARIES ${BREAKPAD_COMMON_LIBRARY} ${BREAKPAD_EXCEPTION_HANDLER_LIBRARIES} ${BREAKPAD_CRASH_GEN_LIBRARY})
        else()
            set(BREAKPAD_EXCEPTION_HANDLER_FOUND "NO")
        endif()
    else()
        if(BREAKPAD_DUMP_SYMS_PATH)
            set(BREAKPAD_DUMP_SYMS_EXEC ${BREAKPAD_DUMP_SYMS_PATH})
            set(BREAKPAD_EXCEPTION_HANDLER_FOUND "YES")
        endif()
    endif()
else ()
    set(BREAKPAD_EXCEPTION_HANDLER_FOUND "NO")
endif ()


include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set BREAKPAD_FOUND to TRUE if
# all listed variables are TRUE
find_package_handle_standard_args(breakpad
                                  REQUIRED_VARS BREAKPAD_EXCEPTION_HANDLER_LIBRARIES BREAKPAD_EXCEPTION_HANDLER_INCLUDE_DIR)

mark_as_advanced(
  BREAKPAD_EXCEPTION_HANDLER_LIBRARY
  BREAKPAD_EXCEPTION_HANDLER_LIBRARIES
  BREAKPAD_EXCEPTION_HANDLER_INCLUDE_DIR
  )
  
if(BREAKPAD_EXCEPTION_HANDLER_FOUND AND NOT TARGET Breakpad::Breakpad)
  add_library(Breakpad::Breakpad UNKNOWN IMPORTED)
  set_target_properties(Breakpad::Breakpad PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${BREAKPAD_EXCEPTION_HANDLER_INCLUDE_DIR}")
  set_property(TARGET Breakpad::Breakpad APPEND PROPERTY IMPORTED_LOCATION "${BREAKPAD_EXCEPTION_HANDLER_LIBRARIES}")
endif()