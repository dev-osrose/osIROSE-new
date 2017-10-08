
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
    ${CMAKE_GEN_SRC_DIR}/version.h
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

function(generate_symbol_data target)
  if(UNIX)
    add_custom_command(TARGET ${target}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/symbols
      COMMAND ${BREAKPAD_EXCEPTION_HANDLER_INSTALL_DIR}/bin/dump_syms $<TARGET_FILE:${target}> > ${CMAKE_BINARY_DIR}/bin/symbols/${target}.sym
      #TODO Generate symbol folders
      VERBATIM
    )
  endif()
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
      COMMAND python
      ARGS ${SQLPP_INCLUDE_DIR}/../scripts/ddl2cpp ${MATCH_PATH} "${OUTPATH}/${ARG_NAME}" ${ARG_NAME}
      DEPENDS ${ABS_FILE}
      COMMENT "Running C++ sqlpp11 ddl12cpp compiler on ${MATCH_PATH} with root ${SQLROOT}, generating: ${H_FILE}"
      VERBATIM)

  ENDFOREACH()

  SET_SOURCE_FILES_PROPERTIES(${${HDRS}} PROPERTIES GENERATED TRUE)
  SET(${HDRS} ${${HDRS}} PARENT_SCOPE)

endfunction()

#-----------------------------------------------------------------------------
# CMake module helper functions
#-----------------------------------------------------------------------------
function(add_copy_target_file_step target target_loc)
  get_filename_component(target_name "${target_loc}" NAME)

  if(${target_name} MATCHES ".*\\.exe")
    if(NOT IS_ABSOLUTE ${BINDIR})
      add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/${BINDIR}")
      add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${target_loc}" "${CMAKE_BINARY_DIR}/${BINDIR}/${target_name}")
    endif()
  elseif(${target_name} MATCHES ".*\\.dll" OR ${target_name} MATCHES ".*\\.so.*" OR ${target_name} MATCHES ".*\\.dylib" OR ${target_name} MATCHES ".*\\.sh")
    if(NOT IS_ABSOLUTE ${RUNTIME_LIBDIR})
      add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/${RUNTIME_LIBDIR}")

      if(WIN32)
        add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${target_loc}" "${CMAKE_BINARY_DIR}/${RUNTIME_LIBDIR}/${target_name}")
      else()
        add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${target_loc}" "${CMAKE_BINARY_DIR}/${RUNTIME_LIBDIR}/${target_name}")
      endif()

    endif()
  elseif(${target_name} MATCHES ".*\\.lib" OR ${target_name} MATCHES ".*\\.a")
    
  else()
    # handle UNIX executeables here
    # get the extension first
    get_filename_component(target_ext ${target_name} EXT)
    if("${target_ext}" STREQUAL "")
      add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/${RUNTIME_LIBDIR}")
      add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${target_loc}" "${CMAKE_BINARY_DIR}/${RUNTIME_LIBDIR}/${target_name}")
    endif()
  endif()
endfunction()

function(add_copy_target_dir_step target target_loc)
	get_filename_component(target_name "${target_loc}" NAME)
	add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${target_loc}" "${CMAKE_BINARY_DIR}/${BINDIR}/${target_name}")
endfunction()

function(add_copy_target_step target)
  get_target_property(target_loc ${target} LOCATION)
  add_copy_target_file_step(${target} ${target_loc})
endfunction()

macro(add_compiler_flags)
  set(flags_list "")
  parse_arguments(ARG "" "C;CXX;DEBUG;RELEASE" ${ARGN})

  if(NOT ARG_DEBUG AND NOT ARG_RELEASE)
    foreach(flag ${ARG_DEFAULT_ARGS})
      set(flags_list "${flags_list} ${flag}")
    endforeach()
    if(NOT ARG_C AND NOT ARG_CXX OR ARG_C)
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${flags_list}")
    endif()
    if(NOT ARG_C AND NOT ARG_CXX OR ARG_CXX)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flags_list}")
    endif()
  elseif(ARG_DEBUG)
    foreach(flag ${ARG_DEFAULT_ARGS})
      set(flags_list "${flags_list} ${flag}")
    endforeach()
    if(NOT ARG_C AND NOT ARG_CXX OR ARG_C)
      set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${flags_list}")
    endif()
    if(NOT ARG_C AND NOT ARG_CXX OR ARG_CXX)
      set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${flags_list}")
    endif()
  elseif(ARG_RELEASE)
    foreach(flag ${ARG_DEFAULT_ARGS})
      set(flags_list "${flags_list} ${flag}")
    endforeach()
    if(NOT ARG_C AND NOT ARG_CXX OR ARG_C)
      set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${flags_list}")
      set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${flags_list}")
    endif()
    if(NOT ARG_C AND NOT ARG_CXX OR ARG_CXX)
      set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${flags_list}")
      set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${flags_list}")
    endif()
  endif()
endmacro()

macro(add_linker_flags)
  foreach(flag ${ARGN})
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${flag}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${flag}")
    set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} ${flag}")
  endforeach()
endmacro()

macro(set_up_32bit_env)
  if(CMAKE_COMPILER_IS_GNUCXX)
    add_compiler_flags(-m32)
    add_linker_flags(-m32)
  endif()
endmacro()

macro(pair)
  set(_aList ${ARGN})
  list(GET _aList 0 _CMD)
  
  if(_CMD STREQUAL "FIRST")
    list(GET _aList 1 _in)
    list(GET _aList 2 _out)
    string(FIND "${_in}" "->" _pos)
    string(SUBSTRING "${_in}" 0 ${_pos} ${_out})
    
    unset(_in)
    unset(_out)
    unset(_pos)
  elseif(_CMD STREQUAL "SECOND")
    list(GET _aList 1 _in)
    list(GET _aList 2 _out)
    string(FIND "${_in}" "->" _pos)
    string(LENGTH "${_in}" _len)

    math(EXPR _pos "${_pos} + 2")
    math(EXPR _len "${_len} - ${_pos}")

    string(SUBSTRING "${_in}" ${_pos} ${_len} ${_out})
    
    unset(_in)
    unset(_out)
    unset(_pos)
    unset(_len)
  else()
    message(FATAL_ERROR "${_CMD} is not implemented for pair")
  endif()
  
  unset(_CMD)
  unset(_aList)
endmacro()


function(add_gtest_test binaryname arg)
  if(WITH_GTEST)
    add_test(NAME ${binaryname}_gtest COMMAND "${CMAKE_BINARY_DIR}/${BINDIR}/${binaryname}" ${arg})
  endif()
endfunction()

function(__internal_install_binary target category create_wrapper)
  add_copy_target_step(${target})
  set_property(TARGET ${target} PROPERTY FOLDER "${category}")

  if(WIN32)
    install(TARGETS ${target}
            RUNTIME DESTINATION "${BIN_INSTALL_DIR}")
  else()
    install(TARGETS ${target}
            RUNTIME DESTINATION "${LIB_INSTALL_DIR}")
    if(create_wrapper)
      # install script to launch binary
      set(CURRENT_TARGET "${target}")
      configure_file("${CMAKE_SCRIPT_PATH}/run.sh" "${CMAKE_BINARY_DIR}/${CURRENT_TARGET}" @ONLY)
      install(FILES "${CMAKE_BINARY_DIR}/${CURRENT_TARGET}"
              DESTINATION "${BIN_INSTALL_DIR}"
              PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                          GROUP_READ             GROUP_EXECUTE
                          WORLD_READ             WORLD_EXECUTE)
    endif()
  endif()
endfunction()

function(install_executable target)
  __internal_install_binary(${target} "Executable" FALSE)
endfunction()

function(install_tool target)
  __internal_install_binary(${target} "Tools" TRUE)
endfunction()

function(install_internal_tool target)
  __internal_install_binary(${target} "Tools" FALSE)
endfunction()

function(install_internal_tool_script file)
  install(FILES ${file}
          DESTINATION ${LIB_INSTALL_DIR}
          PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                      GROUP_READ             GROUP_EXECUTE
                      WORLD_READ             WORLD_EXECUTE
  )
endfunction()

function(install_external_library target)
  foreach(file ${ARGN})
    install(CODE "get_filename_component(resolved_file \"${file}\" REALPATH)\n get_filename_component(file_name \"${file}\" NAME)\n file(INSTALL DESTINATION ${LIB_INSTALL_DIR} TYPE FILE RENAME \${file_name} FILES \"\${resolved_file}\" )")
    set_property(TARGET ${target} PROPERTY FOLDER "ThirdParty")
    add_copy_target_file_step(${target} "${file}")
  endforeach()
endfunction()

function(install_third_party_library target)
  install(TARGETS ${target} DESTINATION ${LIB_INSTALL_DIR})
  set_property(TARGET ${target} PROPERTY FOLDER "ThirdParty")
  add_copy_target_step(${target})
endfunction()

function(install_library target)
  # dlls are runtime targets, import libs are archive, but we don't need them
  install(TARGETS "${target}"
          RUNTIME DESTINATION "${LIB_INSTALL_DIR}"
          LIBRARY DESTINATION "${LIB_INSTALL_DIR}")
  add_copy_target_step(${target})
  set_property(TARGET ${target} PROPERTY FOLDER "Shared")  
endfunction()

macro(generate_current_data_copy_target target)
  file(RELATIVE_PATH __rel_current_dir "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
  set(${target} "internal_data_copy_${__rel_current_dir}")
endmacro()

function(__create__data_copy_target)
  generate_current_data_copy_target(target)
  if(NOT TARGET ${target})
    add_custom_target(${target} ALL)
  endif()
endfunction()

function(add_copy_data_file_step file new_name path)
  if(NOT IS_ABSOLUTE "${DATADIR}")
    generate_current_data_copy_target(target)
    add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${file}" "${CMAKE_BINARY_DIR}/${DATADIR}/${path}/${new_name}" COMMENT "copy ${file} to ${DATADIR}/${path}/${new_name}")
  endif()
endfunction()

function(install_data_files target_path)
  __create__data_copy_target()
  foreach(file ${ARGN})
    get_filename_component(file_name "${file}" NAME)
    set(new_name "${file_name}")
    # remove _win and _lin stuff
    if("${file_name}" MATCHES ".*_[wl]in.*")
      string(REGEX REPLACE "_[wl]in" "" new_name "${file_name}")
    endif()
    add_copy_data_file_step("${file}" "${new_name}" "${target_path}")
    install(FILES "${file}" RENAME "${new_name}" DESTINATION "${DATA_INSTALL_DIR}/${target_path}")
  endforeach()
endfunction()

function(install_data_directories target_path)
  __create__data_copy_target()
  foreach(dir ${ARGN})
    get_filename_component(target_dir_name "${dir}" NAME)
    file(GLOB_RECURSE files "${dir}" "${dir}/*")
    foreach(file ${files})
      get_filename_component(file_name "${file}" NAME)
      file(RELATIVE_PATH rel_file "${dir}" "${file}")
      get_filename_component(rel_path "${rel_file}" PATH)
      add_copy_data_file_step("${file}" "${file_name}" "${target_path}/${target_dir_name}/${rel_path}")
    endforeach()
    install(DIRECTORY ${dir}
            DESTINATION "${DATA_INSTALL_DIR}/${target_path}")
  endforeach()
endfunction()

macro(link_with_gtest target)
  if(WITH_GTEST)
    if(MSVC11)
      add_definitions(-D_VARIADIC_MAX=10)
    endif()

	if (WITH_GMOCK)
      include_directories(${GMOCK_INCLUDE_DIRS})
	  include_directories(${GTEST_INCLUDE_DIRS})
      target_link_libraries(${target} ${GMOCK_LIBRARIES})
	  add_definitions(-DLINK_WITH_GMOCK)
	  add_definitions(-DLINK_WITH_GTEST)
	  if (NOT GMOCK_FOUND)
	    add_dependencies(${target} gmock)
	  endif()
	else()
	  add_definitions(-DLINK_WITH_GTEST)
      include_directories(${GTEST_INCLUDE_DIRS})
      target_link_libraries(${target} ${GTEST_LIBRARIES})
	  if (NOT GTEST_FOUND)
	    add_dependencies(${target} gtest)
	  endif()	  
	endif()
  endif()
endmacro()

MACRO(setup_precompiled_header PrecompiledHeader PrecompiledSource SourcesVar)
  IF(MSVC)
    GET_FILENAME_COMPONENT(PrecompiledBasename ${PrecompiledHeader} NAME_WE)
    SET(PrecompiledBinary "${CMAKE_CURRENT_BINARY_DIR}/${PrecompiledBasename}.pch")
    SET(SourcesInternal ${${SourcesVar}})

    SET_SOURCE_FILES_PROPERTIES(${PrecompiledSource}
                                PROPERTIES COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_OUTPUTS "${PrecompiledBinary}")
    SET_SOURCE_FILES_PROPERTIES(${SourcesInternal}
                                PROPERTIES COMPILE_FLAGS "/Yu\"${PrecompiledHeader}\" /FI\"${PrecompiledBinary}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_DEPENDS "${PrecompiledBinary}")  
    # Add precompiled header to SourcesVar
    LIST(APPEND ${SourcesVar} ${PrecompiledSource})
  ENDIF(MSVC)
ENDMACRO(setup_precompiled_header)

message(STATUS "osIROSE cmake macros loaded.")
