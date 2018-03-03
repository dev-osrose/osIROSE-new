
IF( DISABLE_CRYPT )
  add_compiler_flags(-DDISABLE_CRYPT)
ENDIF()

include_directories(
  SYSTEM ${GTEST_INCLUDE_DIRS}
  SYSTEM ${ASIO_INCLUDE_DIR}
  SYSTEM ${EXTERNAL_INCLUDE_DIR}
  SYSTEM ${SPDLOG_ROOT}
  SYSTEM ${SOL2_ROOT}
  SYSTEM ${LUA_INCLUDE_DIR}
  SYSTEM ${CXXOPT_ROOT}
  SYSTEM ${ENTITYX_INCLUDE_DIR}
  SYSTEM ${SQLPP_INCLUDE_DIR}
  SYSTEM ${DATE_INCLUDE_DIR}
  SYSTEM ${CONFIGURU}
  SYSTEM ${VISIT_STRUCT}
)

IF(WITH_MYSQL)
  add_compiler_flags(-DENABLE_MYSQL)
  
  include_directories(
    SYSTEM ${MYSQL_INCLUDE_DIRS}
    SYSTEM ${SQLPP_CONNECTOR_MYSQL_INCLUDE_DIR}
  )

  link_directories(
    ${MYSQL_LIBRARY_PATH}
  )
ENDIF()

if(WITH_CRASH_REPORTS)
  add_compiler_flags(-DENABLE_CRASH_REPORTS)
  
  include_directories(
    SYSTEM ${BREAKPAD_EXCEPTION_HANDLER_INCLUDE_DIR}
  )
endif()

add_subdirectory(core)
add_subdirectory(rosecommon)
add_subdirectory(login)
add_subdirectory(char)
add_subdirectory(map)

add_subdirectory(bot)

IF(BUILD_TESTS)
  add_subdirectory(tests)
ENDIF()

