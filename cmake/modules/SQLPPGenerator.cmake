function(GENERATE_SQL_HEADER HDRS)
  cmake_parse_arguments(ARG "DEBUG" "SQLROOT;NAME;OUTPATH;EXPORT_MACRO;TARGET" "SQLFILE" ${ARGN})

  if(NOT ARG_SQLFILE)
    message(SEND_ERROR "Error: GENERATE_SQL_HEADER() called without any sql files")
    return()
  endif(NOT ARG_SQLFILE)

  if(NOT ARG_NAME)
    message(SEND_ERROR "Error: GENERATE_SQL_HEADER() called without any class name")
    return()
  endif(NOT ARG_NAME)

  list(LENGTH ARG_SQLROOT SQLROOT_LENGTH)
  if(SQLROOT_LENGTH GREATER 1)
    message(SEND_ERROR "Error: GENERATE_SQL_HEADER() called with too many sqlroots, only one is allowed")
    return()
  endif()

  list(LENGTH ARG_OUTPATH OUTPATH_LENGTH)
  if(OUTPATH_LENGTH GREATER 1)
    message(SEND_ERROR "Error: GENERATE_SQL_HEADER() called with too many outpaths, only one is allowed")
    return()
  endif()

  set(OUTPATH ${CMAKE_CURRENT_BINARY_DIR})
  if(OUTPATH_LENGTH EQUAL 1)
    set(OUTPATH ${ARG_OUTPATH})
  endif()

  set(SQLROOT ${CMAKE_CURRENT_SOURCE_DIR})
  if(SQLROOT_LENGTH EQUAL 1)
    set(SQLROOT ${ARG_SQLROOT})
  endif()

  if(ARG_DEBUG)
    message("OUTPATH: ${OUTPATH}")
    message("SQLFILE: ${ARG_SQLFILE}")
  endif()

  set(${HDRS})
  foreach(SQLFILE ${ARG_SQLFILE})

    # ensure that the file ends with .proto
    string(REGEX MATCH "\\.sql$$" PROTOEND ${SQLFILE})
    if(NOT PROTOEND)
      message(SEND_ERROR "Sql file '${SQLFILE}' does not end with .sql")
    endif()

    get_filename_component(SQL_PATH ${SQLFILE} PATH)
    get_filename_component(ABS_FILE ${SQLFILE} ABSOLUTE)
    get_filename_component(FILE_WE ${SQLFILE} NAME_WE)

    if(ARG_DEBUG)
      message("file ${SQLFILE}:")
      message("  PATH=${SQL_PATH}")
      message("  ABS_FILE=${ABS_FILE}")
      message("  FILE_WE=${FILE_WE}")
      message("  SQLROOT=${SQLROOT}")
    endif()

    # find out of the file is in the specified proto root
    # TODO clean the SQLROOT so that it does not form a regex itself?
    string(REGEX MATCH "^${SQLROOT}" IN_ROOT_PATH ${SQLFILE})
    string(REGEX MATCH "^${SQLROOT}" IN_ROOT_ABS_FILE ${ABS_FILE})

    if(IN_ROOT_PATH)
      set(MATCH_PATH ${SQLFILE})
    elseif(IN_ROOT_ABS_FILE)
      set(MATCH_PATH ${ABS_FILE})
    else()
      message(SEND_ERROR "Sql file '${SQLFILE}' is not in sqlroot '${SQLROOT}'")
    endif()

    # build the result file name
    string(REGEX REPLACE "^${SQLROOT}(/?)" "" ROOT_CLEANED_FILE ${MATCH_PATH})
    if(ARG_DEBUG)
      message("  ROOT_CLEANED_FILE=${ROOT_CLEANED_FILE}")
    endif()
    string(REGEX REPLACE "\\.sql$$" "" EXT_CLEANED_FILE ${ROOT_CLEANED_FILE})
    if(ARG_DEBUG)
        message("  EXT_CLEANED_FILE=${EXT_CLEANED_FILE}")
    endif()

    set(H_FILE "${OUTPATH}/${ARG_NAME}.h")

    if(ARG_DEBUG)
      message("  H_FILE=${H_FILE}")
    endif()

    list(APPEND ${HDRS} "${H_FILE}")

    add_custom_command(
      OUTPUT "${H_FILE}"
      COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPATH}
      COMMAND Python::Interpreter
      ARGS ${SQLPP11_EXE_PATH} ${MATCH_PATH} "${OUTPATH}/${ARG_NAME}" ${ARG_NAME}
      DEPENDS ${ABS_FILE} SQLPP11::SQLPP11_MYSQL
      COMMENT "Running C++ sqlpp11 ddl12cpp compiler on ${MATCH_PATH} with root ${SQLROOT}, generating: ${H_FILE}"
      VERBATIM)

  endforeach()
  
  add_custom_target(${ARG_NAME}_generated
    DEPENDS ${${HDRS}}
  )

  set_source_files_properties(${${HDRS}} PROPERTIES GENERATED TRUE)
  set(${HDRS} ${${HDRS}} PARENT_SCOPE)

endfunction()