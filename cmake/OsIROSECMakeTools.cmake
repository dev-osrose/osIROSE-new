MACRO(OSIROSE_CONFIGURE_FILE_AUTOTOOLS_COMPAT)
  SET(configure_input "${ARGV1} generated from ${ARGV0} by CMake.")
  CONFIGURE_FILE(${ARGV})
ENDMACRO(OSIROSE_CONFIGURE_FILE_AUTOTOOLS_COMPAT)


FUNCTION(CREATE_VERSION_FILE)
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
ENDFUNCTION()


MESSAGE(STATUS "osIROSE CMake Tools loaded.")
