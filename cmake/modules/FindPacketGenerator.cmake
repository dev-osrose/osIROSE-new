
find_program( IDL_GEN_EXE_PATH 
            packet_generator
            PATHS
                ${CMAKE_THIRD_PARTY_DIR}/bin
            PATH_SUFFIXES
                target/debug
                target/release )
                

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LUA_FOUND to TRUE if
# all listed variables are TRUE
FIND_PACKAGE_HANDLE_STANDARD_ARGS(IDL
                                  REQUIRED_VARS IDL_GEN_EXE_PATH)
                                  
if(IDL_FOUND AND NOT TARGET idl::packet_generator)
  add_executable(idl::packet_generator IMPORTED GLOBAL)
  add_dependencies(idl::packet_generator ${IDL_GEN_EXE_PATH})
  set_target_properties(idl::packet_generator PROPERTIES IMPORTED_LOCATION ${IDL_GEN_EXE_PATH})
endif()

mark_as_advanced(IDL_GEN_EXE_PATH)