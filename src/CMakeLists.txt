
if( DISABLE_CRYPT )
  add_compiler_flags(-DDISABLE_CRYPT)
endif()

include_directories(
  SYSTEM ${CMAKE_THIRD_PARTY_INCLUDE_DIR}
  SYSTEM ${spdlog_SOURCE_DIR}/include
  SYSTEM ${sol2_SOURCE_DIR}/include
  SYSTEM ${cxxopts_SOURCE_DIR}/include
  SYSTEM ${entt_SOURCE_DIR}/src/entt
  SYSTEM ${SQLPP_INCLUDE_DIR}
  SYSTEM ${DATE_INCLUDE_DIR}
  SYSTEM ${CONFIGURU}
  SYSTEM ${VISIT_STRUCT}
)

link_directories(
  ${CMAKE_THIRD_PARTY_LIBRARY_DIR}
)

if(WITH_MARIADB)
  add_compiler_flags(-DENABLE_MARIADB)
  include_directories(
    SYSTEM ${MYSQL_INCLUDE_DIR}
    SYSTEM ${SQLPP_CONNECTOR_MYSQL_INCLUDE_DIR}
  )
  link_directories(
    ${MYSQL_LIBRARY_PATH}
  )
elseif(WITH_MYSQL)
  add_compiler_flags(-DENABLE_MYSQL)
  
  include_directories(
    SYSTEM ${MYSQL_INCLUDE_DIR}
    SYSTEM ${SQLPP_CONNECTOR_MYSQL_INCLUDE_DIR}
  )

  link_directories(
    ${MYSQL_LIBRARY_PATH}
  )
endif()

add_subdirectory(core)
add_subdirectory(rosecommon)
add_subdirectory(node)
add_subdirectory(login)
add_subdirectory(char)
add_subdirectory(map)

if(BUILD_TOOLS)
  add_subdirectory(bot)
endif()

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()

