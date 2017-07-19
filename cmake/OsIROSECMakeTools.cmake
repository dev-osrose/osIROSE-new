MACRO(PARSE_ARGUMENTS prefix arg_names option_names)
    SET(DEFAULT_ARGS)
    FOREACH(arg_name ${arg_names})
        SET(${prefix}_${arg_name})
    ENDFOREACH(arg_name)
    FOREACH(option ${option_names})
        SET(${prefix}_${option} FALSE)
    ENDFOREACH(option)

    SET(current_arg_name DEFAULT_ARGS)
    SET(current_arg_list)
    FOREACH(arg ${ARGN})
        SET(larg_names ${arg_names})
        LIST(FIND larg_names "${arg}" is_arg_name)
        IF (is_arg_name GREATER -1)
            SET(${prefix}_${current_arg_name} ${current_arg_list})
            SET(current_arg_name ${arg})
            SET(current_arg_list)
        ELSE (is_arg_name GREATER -1)
            SET(loption_names ${option_names})
            LIST(FIND loption_names "${arg}" is_option)
            IF (is_option GREATER -1)
                SET(${prefix}_${arg} TRUE)
            ELSE (is_option GREATER -1)
                SET(current_arg_list ${current_arg_list} ${arg})
            ENDIF (is_option GREATER -1)
        ENDIF (is_arg_name GREATER -1)
    ENDFOREACH(arg)
    SET(${prefix}_${current_arg_name} ${current_arg_list})
ENDMACRO(PARSE_ARGUMENTS)

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


function(GENERATE_SQL_HEADER HDRS)
  PARSE_ARGUMENTS(ARG "SQLROOT;SQLFILE;OUTPATH;NAME;TARGET;EXPORT_MACRO" "DEBUG" ${ARGN})

  IF(NOT ARG_SQLFILE)
    MESSAGE(SEND_ERROR "Error: GENERATE_SQL_HEADER() called without any sql files")
    RETURN()
  ENDIF(NOT ARG_SQLFILE)

  IF(NOT ARG_NAME)
    MESSAGE(SEND_ERROR "Error: GENERATE_SQL_HEADER() called without any class name")
    RETURN()
  ENDIF(NOT ARG_NAME)

  LIST(LENGTH ARG_SQLROOT SQLROOT_LENGTH)
  IF(SQLROOT_LENGTH GREATER 1)
    MESSAGE(SEND_ERROR "Error: GENERATE_SQL_HEADER() called with too many sqlroots, only one is allowed")
    RETURN()
  ENDIF()

  LIST(LENGTH ARG_OUTPATH OUTPATH_LENGTH)
  IF(OUTPATH_LENGTH GREATER 1)
    MESSAGE(SEND_ERROR "Error: GENERATE_SQL_HEADER() called with too many outpaths, only one is allowed")
    RETURN()
  ENDIF()

  SET(OUTPATH ${CMAKE_CURRENT_BINARY_DIR})
  IF(OUTPATH_LENGTH EQUAL 1)
    SET(OUTPATH ${ARG_OUTPATH})
  ENDIF()

  SET(SQLROOT ${CMAKE_CURRENT_SOURCE_DIR})
  IF(SQLROOT_LENGTH EQUAL 1)
    SET(SQLROOT ${ARG_SQLROOT})
  ENDIF()

  IF(ARG_DEBUG)
    MESSAGE("OUTPATH: ${OUTPATH}")
    MESSAGE("SQLFILE: ${ARG_SQLFILE}")
  ENDIF()

  SET(${HDRS})
  FOREACH(SQLFILE ${ARG_SQLFILE})

    # ensure that the file ends with .proto
    STRING(REGEX MATCH "\\.sql$$" PROTOEND ${SQLFILE})
    IF(NOT PROTOEND)
        MESSAGE(SEND_ERROR "Sql file '${SQLFILE}' does not end with .sql")
    ENDIF()

    GET_FILENAME_COMPONENT(SQL_PATH ${SQLFILE} PATH)
    GET_FILENAME_COMPONENT(ABS_FILE ${SQLFILE} ABSOLUTE)
    GET_FILENAME_COMPONENT(FILE_WE ${SQLFILE} NAME_WE)

    IF(ARG_DEBUG)
      MESSAGE("file ${SQLFILE}:")
      MESSAGE("  PATH=${SQL_PATH}")
      MESSAGE("  ABS_FILE=${ABS_FILE}")
      MESSAGE("  FILE_WE=${FILE_WE}")
      MESSAGE("  SQLROOT=${SQLROOT}")
    ENDIF()

    # find out of the file is in the specified proto root
    # TODO clean the SQLROOT so that it does not form a regex itself?
    STRING(REGEX MATCH "^${SQLROOT}" IN_ROOT_PATH ${SQLFILE})
    STRING(REGEX MATCH "^${SQLROOT}" IN_ROOT_ABS_FILE ${ABS_FILE})

    IF(IN_ROOT_PATH)
      SET(MATCH_PATH ${SQLFILE})
    ELSEIF(IN_ROOT_ABS_FILE)
      SET(MATCH_PATH ${ABS_FILE})
    ELSE()
      MESSAGE(SEND_ERROR "Sql file '${SQLFILE}' is not in sqlroot '${SQLROOT}'")
    ENDIF()

    # build the result file name
    STRING(REGEX REPLACE "^${SQLROOT}(/?)" "" ROOT_CLEANED_FILE ${MATCH_PATH})
    IF(ARG_DEBUG)
      MESSAGE("  ROOT_CLEANED_FILE=${ROOT_CLEANED_FILE}")
    ENDIF()
    STRING(REGEX REPLACE "\\.sql$$" "" EXT_CLEANED_FILE ${ROOT_CLEANED_FILE})
    IF(ARG_DEBUG)
        MESSAGE("  EXT_CLEANED_FILE=${EXT_CLEANED_FILE}")
    ENDIF()

    SET(H_FILE "${OUTPATH}/${ARG_NAME}.h")

    IF(ARG_DEBUG)
      MESSAGE("  H_FILE=${H_FILE}")
    ENDIF()

    LIST(APPEND ${HDRS} "${H_FILE}")

    ADD_CUSTOM_COMMAND(
      OUTPUT "${H_FILE}"
      COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPATH}
      COMMAND "/usr/bin/env"
      ARGS python ${SQLPP_INCLUDE_DIR}/../scripts/ddl2cpp ${MATCH_PATH} "${OUTPATH}/${ARG_NAME}" ${ARG_NAME}
      DEPENDS ${ABS_FILE}
      COMMENT "Running C++ sqlpp11 ddl12cpp compiler on ${MATCH_PATH} with root ${SQLROOT}, generating: ${H_FILE}"
      VERBATIM)

  ENDFOREACH()

  SET_SOURCE_FILES_PROPERTIES(${${HDRS}} PROPERTIES GENERATED TRUE)
  SET(${HDRS} ${${HDRS}} PARENT_SCOPE)

endfunction()



message(STATUS "osIROSE CMake Tools loaded.")
