set(LUA_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

if(WIN32)
else()
  set(_byproducts
    ${LUA_INSTALL_DIR}/lib/liblua.a
  )
  ExternalProject_Add(
    lua
    URL https://www.lua.org/ftp/lua-5.3.4.tar.gz
    URL_HASH SHA1=79790cfd40e09ba796b01a571d4d63b52b1cd950
    INSTALL_DIR ${LUA_INSTALL_DIR}
    BUILD_IN_SOURCE TRUE
    
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make linux
    INSTALL_COMMAND make install INSTALL_TOP=<INSTALL_DIR>
    BUILD_BYPRODUCTS ${_byproducts}
  )
endif()

ExternalProject_Get_Property(
  lua
  install_dir
)

set(LUA_INCLUDE_DIRS "${install_dir}/include")

if(WIN32)
#  set(LUA_LIBRARY "$<$<CONFIG:Release>:${install_dir}/lib/liblua.lib>$<$<CONFIG:Debug>:${install_dir}/lib/liblua.lib>")
#  set(LUA_LIBRARIES "${CURL_LIBRARY}")
#  set(LUA_INSTALL_LIBS "$<$<CONFIG:Release>:${install_dir}/bin/liblua.dll>$<$<CONFIG:Debug>:${install_dir}/bin/liblua.dll>")
else()
  set(LUA_LIBRARY "${install_dir}/lib/liblua.a")
  set(LUA_LIBRARIES "${LUA_LIBRARY}")
  set(LUA_INSTALL_LIBS "${install_dir}/lib/liblua.a")
endif()

if(NOT TARGET lua::lua AND NOT WIN32)
    add_library(lua::lua static EXPORT)
    set_target_properties(lua::lua PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LUA_INCLUDE_DIRS}")
    set_property(TARGET lua::lua APPEND PROPERTY IMPORTED_LOCATION "${LUA_LIBRARY}")
endif()