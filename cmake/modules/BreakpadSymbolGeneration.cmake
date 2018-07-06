function(generate_symbol_data target)
  if(WITH_CRASH_REPORTS)
    set(TARGET_PATH ${CMAKE_BINARY_DIR}/bin/symbols/${target}.sym)

    if(UNIX)
      add_custom_command(TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/symbols
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/symbols/${target}
        COMMAND ${BREAKPAD_DUMP_SYMS_EXEC} $<TARGET_FILE:${target}> > ${TARGET_PATH}
        COMMAND head -n1 ${TARGET_PATH} > ${CMAKE_CURRENT_BINARY_DIR}/${target}.str
        COMMAND bash ${CMAKE_SCRIPT_PATH}/create_breakpad_directories.sh ${CMAKE_BINARY_DIR}/bin/symbols/${target} ${CMAKE_CURRENT_BINARY_DIR}/${target}.str ${TARGET_PATH}
        VERBATIM
      )
    elseif(MSVC)
      add_custom_command(TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/symbols
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/symbols/${target}.pdb
        COMMAND ${BREAKPAD_DUMP_SYMS_EXEC} $<TARGET_FILE:${target}> > ${TARGET_PATH}
        COMMAND ${CMAKE_SCRIPT_PATH}/create_breakpad_directories.bat ${CMAKE_BINARY_DIR}/bin/symbols/${target}.pdb ${TARGET_PATH}
        VERBATIM
      )# TODO create the correct breakpad directories for the symbols
    endif()
  else()
    MESSAGE("-- crash reports are disabled. skipping symbol generation for ${target}")
  endif()
endfunction()