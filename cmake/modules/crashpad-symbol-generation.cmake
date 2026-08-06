#------------------------------------------------------------------------------
# Symbol data for crashpad minidumps
#
# A minidump only records addresses; symbolising one needs the debug info for
# the exact binary that produced it. Breakpad shipped dump_syms to convert that
# into .sym text files, but crashpad has no such tool - the modern flow feeds
# the debug info straight to the symbol server (sentry-cli upload-dif,
# backtrace-morgue, ...). So all we do here is split the debug info out into a
# file that can be archived next to the release and uploaded later.
#
# The binary is deliberately left unstripped: stripping it would break local
# gdb sessions, and the servers are not size constrained.
#------------------------------------------------------------------------------
function(generate_crash_symbols target)
  if (NOT WITH_CRASH_REPORTS)
    message(STATUS "crash reports are disabled - skipping symbol generation for ${target}")
    return()
  endif ()

  if (MSVC)
    # MSVC already emits a .pdb next to the binary (see the /Zi and /DEBUG
    # flags set in cmake/platform/win32/platform.cmake), which is exactly what
    # a symbol server wants.
    return()
  endif ()

  if (NOT CMAKE_OBJCOPY)
    message(WARNING "objcopy not found - no debug symbols will be split out for ${target}")
    return()
  endif ()

  set(SYMBOL_DIR ${CMAKE_BINARY_DIR}/bin/symbols)
  set(SYMBOL_FILE ${SYMBOL_DIR}/${target}.debug)

  add_custom_command(TARGET ${target}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E make_directory ${SYMBOL_DIR}
      COMMAND ${CMAKE_OBJCOPY} --only-keep-debug $<TARGET_FILE:${target}> ${SYMBOL_FILE}
      BYPRODUCTS ${SYMBOL_FILE}
      COMMENT "Extracting debug symbols for ${target}"
      VERBATIM
      )
endfunction()
