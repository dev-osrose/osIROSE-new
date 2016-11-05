macro(OSIROSE_CONFIGURE_FILE_AUTOTOOLS_COMPAT)
  set(configure_input "${ARGV1} generated from ${ARGV0} by CMake.")
  configure_file(${ARGV})
endmacro(OSIROSE_CONFIGURE_FILE_AUTOTOOLS_COMPAT)


function(CREATE_VERSION_FILE)
  # Create a version file using git data
  if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    execute_process(
      COMMAND git rev-parse --abbrev-ref HEAD
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE GIT_BRANCH
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  
    execute_process(
      COMMAND git log -1 --format=%h
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE GIT_COMMIT_HASH
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  else()
    set(GIT_BRANCH "")
    set(GIT_COMMIT_HASH "")
  endif()
  # --------------------------
  
  #start version file
  message(STATUS "Generating version.h")
  configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/version.h.in
    ${CMAKE_BINARY_DIR}/generated/version.h
  )
  #end version file
endfunction()

function(OVERWRITE_COMPILER_SETTINGS)
# --------------------------
# Make sure our settings are correct
  foreach(flag_var
      CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)

    if(${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MD")

    if(${flag_var} MATCHES "_DLL")
      string(REGEX REPLACE "_DLL" "" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "_DLL")
  endforeach(flag_var)
# --------------------------
endfunction()


message(STATUS "osIROSE CMake Tools loaded.")
