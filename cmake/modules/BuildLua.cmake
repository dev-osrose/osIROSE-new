set(LUA_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

if(WIN32)
  set(_byproducts
    ${LUA_INSTALL_DIR}/lib/liblua.dll
  )
  ExternalProject_Add(
    lua
    URL https://www.lua.org/ftp/lua-5.3.4.tar.gz
    URL_HASH SHA1=79790cfd40e09ba796b01a571d4d63b52b1cd950
    INSTALL_DIR ${LUA_INSTALL_DIR}
    BUILD_IN_SOURCE TRUE
    
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    BUILD_BYPRODUCTS ${_byproducts}
  )
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

if(WIN32)
#  set(LUA_LIBRARY "$<$<CONFIG:Release>:${install_dir}/lib/liblua.lib>$<$<CONFIG:Debug>:${install_dir}/lib/liblua.lib>")
#  set(LUA_LIBRARIES "${CURL_LIBRARY}")
#  set(LUA_INSTALL_LIBS "$<$<CONFIG:Release>:${install_dir}/bin/liblua.dll>$<$<CONFIG:Debug>:${install_dir}/bin/liblua.dll>")
else()
  set(LUA_LIBRARY "${install_dir}/lib/liblua.a")
  set(LUA_LIBRARIES "${LUA_LIBRARY}")
  set(LUA_INSTALL_LIBS "${install_dir}/lib/liblua.a")
endif()


set(LUA_INCLUDE_DIR "${install_dir}/include")

if(NOT TARGET lua::lua)
    add_library(lua::lua STATIC IMPORTED)
    set_target_properties(lua::lua PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LUA_INCLUDE_DIR}")
    set_property(TARGET lua::lua APPEND PROPERTY IMPORTED_LOCATION "${LUA_LIBRARY}")
endif()

mark_as_advanced(LUA_INCLUDE_DIR LUA_LIBRARY)
