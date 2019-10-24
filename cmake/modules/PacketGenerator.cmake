function(GENERATE_PACKETS SRCS HDRS)
  cmake_parse_arguments(ARG "DEBUG" "IDLROOT;HDR_OUTPATH;SRC_OUTPATH;TARGET" "IDLFILES" ${ARGN})

  if(NOT ARG_IDLFILES)
    message(SEND_ERROR "Error: GENERATE_PACKETS() called without any idl files")
    return()
  endif(NOT ARG_IDLFILES)
  
  set(SRC_OUTPATH ${ARG_SRC_OUTPATH})
  set(HDR_OUTPATH ${ARG_HDR_OUTPATH})
  set(IDLROOT ${ARG_IDLROOT})
  set(TARGET ${ARG_TARGET})

  if(ARG_DEBUG)
    message("SRC_OUTPATH: ${SRC_OUTPATH}")
    message("HDR_OUTPATH: ${HDR_OUTPATH}")
    message("IDLFILES: ${IDLROOT}")
    message("TARGET: ${TARGET}")
  endif()

  set(${SRCS})
  set(${HDRS})
  foreach(IDLFILES ${ARG_IDLFILES})

    # ensure that the file ends with .proto
    string(REGEX MATCH "\\.xml$$" PROTOEND ${IDLFILES})
    if(NOT PROTOEND)
      message(SEND_ERROR "idl file '${IDLFILES}' does not end with .xml")
    endif()

    get_filename_component(IDL_PATH ${IDLFILES} PATH)
    get_filename_component(ABS_FILE ${IDLFILES} ABSOLUTE)
    get_filename_component(FILE_WE ${IDLFILES} NAME_WE)

    if(ARG_DEBUG)
      message("file ${IDLFILES}:")
      message("  PATH=${IDL_PATH}")
      message("  ABS_FILE=${ABS_FILE}")
      message("  FILE_WE=${FILE_WE}")
      message("  IDLROOT=${IDLROOT}")
    endif()

    # find out of the file is in the specified proto root
    # TODO clean the IDLROOT so that it does not form a regex itself?
    string(REGEX MATCH "^${IDLROOT}" IN_ROOT_PATH ${IDLFILES})
    string(REGEX MATCH "^${IDLROOT}" IN_ROOT_ABS_FILE ${ABS_FILE})

    if(IN_ROOT_PATH)
      set(MATCH_PATH ${IDLFILES})
    elseif(IN_ROOT_ABS_FILE)
      set(MATCH_PATH ${ABS_FILE})
    else()
      message(SEND_ERROR "IDL file '${IDLFILES}' is not in IDLROOT '${IDLROOT}'")
    endif()

    # build the result file name
    string(REGEX REPLACE "^${IDLROOT}(/?)" "" ROOT_CLEANED_FILE ${MATCH_PATH})
    if(ARG_DEBUG)
      message("  ROOT_CLEANED_FILE=${ROOT_CLEANED_FILE}")
    endif()
    string(REGEX REPLACE "\\.xml$$" "" EXT_CLEANED_FILE ${ROOT_CLEANED_FILE})
    if(ARG_DEBUG)
        message("  EXT_CLEANED_FILE=${EXT_CLEANED_FILE}")
    endif()

    set(CXX_FILE "${SRC_OUTPATH}/${EXT_CLEANED_FILE}.cpp")
    set(H_FILE "${HDR_OUTPATH}/${EXT_CLEANED_FILE}.h")

    if(ARG_DEBUG)
      message("  CXX_FILE=${CXX_FILE}")
      message("  H_FILE=${H_FILE}")
    endif()

    list(APPEND ${SRCS} "${CXX_FILE}")
    list(APPEND ${HDRS} "${H_FILE}")

    add_custom_command(
      OUTPUT "${H_FILE}" "${CXX_FILE}"
      COMMAND ${CMAKE_COMMAND} -E make_directory ${SRC_OUTPATH}
      COMMAND ${CMAKE_COMMAND} -E make_directory ${HDR_OUTPATH}
      COMMAND utils::packet_generator "${MATCH_PATH}" -h "${HDR_OUTPATH}" -c "${SRC_OUTPATH}"
      DEPENDS ${ABS_FILE} utils::packet_generator
      COMMENT "Running C++ packetGenerator compiler on ${MATCH_PATH} with root ${IDLROOT}, generating: ${CXX_FILE}, ${H_FILE}"
      VERBATIM)
  endforeach()
  
  add_custom_target(${TARGET}_generated
    DEPENDS utils::packet_generator ${${SRCS}} ${${HDRS}}
  )
  
  set_source_files_properties(${${SRCS}} PROPERTIES GENERATED TRUE)
  set(${SRCS} ${${SRCS}} PARENT_SCOPE)

  set_source_files_properties(${${HDRS}} PROPERTIES GENERATED TRUE)
  set(${HDRS} ${${HDRS}} PARENT_SCOPE)

endfunction()