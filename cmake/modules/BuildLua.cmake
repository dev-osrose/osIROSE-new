set(LUA_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

if(WIN32)
  set(_byproducts
    ${LUA_INSTALL_DIR}/bin/lua51.dll
    ${LUA_INSTALL_DIR}/lib/lua51.lib
    ${LUA_INSTALL_DIR}/bin/lua5.1.dll
    ${LUA_INSTALL_DIR}/lib/lua5.1.lib
  )
  
  ExternalProject_Add(
    lua
    GIT_REPOSITORY https://github.com/rjpcomputing/luaforwindows.git
    GIT_TAG v5.1.5-52
    GIT_SHALLOW true
    INSTALL_DIR ${LUA_INSTALL_DIR}
    BUILD_IN_SOURCE TRUE

    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    BUILD_BYPRODUCTS ${_byproducts}
  )
  
  ExternalProject_Add_Step(
    lua
    copy-lua
    DEPENDEES download
    COMMAND ${CMAKE_SCRIPT_PATH}/robocopy.bat "<SOURCE_DIR>/files/include" "<INSTALL_DIR>/include/" "*.h"
    COMMAND ${CMAKE_SCRIPT_PATH}/robocopy.bat "<SOURCE_DIR>/files/include" "<INSTALL_DIR>/include/" "*.hpp"
    COMMAND ${CMAKE_SCRIPT_PATH}/robocopy.bat "<SOURCE_DIR>/files/lib" "<INSTALL_DIR>/lib/" "*.lib"
    COMMAND ${CMAKE_SCRIPT_PATH}/robocopy.bat "<SOURCE_DIR>/files/lib" "<INSTALL_DIR>/bin/" "*.dll"
    COMMAND ${CMAKE_SCRIPT_PATH}/robocopy.bat "<SOURCE_DIR>/files/lib" "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/" "*.dll"
  )
else()
  find_library(LUA_DL_LIBRARY dl)
  find_library(LUA_MATH_LIBRARY m)
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
  set(LUA_LIBRARY "${install_dir}/lib/lua51.lib")
  set(LUA_LIBRARIES "${LUA_LIBRARY}")
  set(LUA_INSTALL_LIBS "${install_dir}/lib/lua51.dll")
else()
  set(LUA_LIBRARY "${install_dir}/lib/liblua.a")
  set(LUA_LIBRARIES "${LUA_LIBRARY}" "${LUA_DL_LIBRARY}" "${LUA_MATH_LIBRARY}")
  set(LUA_INSTALL_LIBS "${install_dir}/lib/liblua.a")
endif()


set(LUA_INCLUDE_DIR "${install_dir}/include")

if(NOT TARGET lua::lua)
    add_library(lua::lua INTERFACE IMPORTED)
    add_dependencies(lua::lua lua)
    set_target_properties(lua::lua PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LUA_INCLUDE_DIR}")
    set_target_properties(lua::lua PROPERTIES INTERFACE_LINK_LIBRARIES "${LUA_LIBRARIES}")
endif()

mark_as_advanced(LUA_INCLUDE_DIR LUA_LIBRARY)
