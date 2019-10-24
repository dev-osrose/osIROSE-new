function(generate_crashpad_symbol_data target)
  if(WITH_CRASH_REPORTS)
    set(TARGET_PATH ${CMAKE_BINARY_DIR}/bin/symbols/${target}.crashpad.sym)

    if(UNIX)
      add_custom_command(TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/symbols
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/symbols/${target}
        COMMAND objcopy --only-keep-debug $<TARGET_FILE:${target}> ${TARGET_PATH}
        #COMMAND strip --strip-debug --strip-unneeded $<TARGET_FILE:${target}>
        #COMMAND objcopy --add-gnu-debuglink="${TARGET_PATH}" "$<TARGET_FILE:${target}>"
        VERBATIM
      )
    elseif(MSVC)
      #Nothing to do since windows creates pdb files nicely
    endif()
  else()
    MESSAGE("-- crash reports are disabled. skipping symbol generation for ${target}")
  endif()
endfunction()