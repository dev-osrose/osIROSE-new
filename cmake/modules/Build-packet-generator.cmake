

set(IDL_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

message(STATUS "Building packet_generator since packet_generator tool was not found")

set(_byproducts
  ${IDL_INSTALL_DIR}/bin/packet_generator
  ${IDL_INSTALL_DIR}/bin/packet_generator.exe
)

if(WIN32 OR MINGW)
  set(DOWNLOAD_URL https://github.com/dev-osrose/IDL/releases/download/idl-latest/win64-packet_generator.zip)
  set(EXEC_POSTFIX .exe)
else()
  set(DOWNLOAD_URL https://github.com/dev-osrose/IDL/releases/download/idl-latest/linux-packet_generator.tar.gz)
  set(EXEC_POSTFIX )
endif()

ExternalProject_Add(
  idl
  URL ${DOWNLOAD_URL}
  UPDATE_DISCONNECTED true
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/packet_generator${EXEC_POSTFIX} <INSTALL_DIR>/bin
  INSTALL_COMMAND ""
  BUILD_BYPRODUCTS ${_byproducts}
  INSTALL_DIR ${IDL_INSTALL_DIR}
)
ExternalProject_Get_Property(
  idl
  install_dir
)

if(WIN32 OR MINGW)
  set(IDL_GEN_EXE_PATH ${install_dir}/bin/packet_generator.exe)
else()
  set(IDL_GEN_EXE_PATH ${install_dir}/bin/packet_generator)
endif()

if(NOT TARGET idl::packet_generator)
  add_executable(idl::packet_generator IMPORTED GLOBAL)
  add_dependencies(idl::packet_generator idl)
  set_target_properties(idl::packet_generator PROPERTIES IMPORTED_LOCATION ${IDL_GEN_EXE_PATH})
endif()

mark_as_advanced( IDL_GEN_EXE_PATH )
