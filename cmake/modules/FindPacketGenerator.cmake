
find_program( IDL_GEN_EXE_PATH 
            NAMES
                packet_generator
            HINTS
                ${CMAKE_THIRD_PARTY_DIR}/bin
            PATH_SUFFIXES
                debug
                release )
                

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set PacketGenerator_FOUND to TRUE if
# all listed variables are TRUE
find_package_handle_standard_args(PacketGenerator
                                  REQUIRED_VARS IDL_GEN_EXE_PATH)

mark_as_advanced(IDL_GEN_EXE_PATH)
                                  
if(PacketGenerator_FOUND AND NOT TARGET utils::packet_generator)
  add_executable(utils::packet_generator IMPORTED GLOBAL)
  set_target_properties(utils::packet_generator PROPERTIES IMPORTED_LOCATION ${IDL_GEN_EXE_PATH})
endif()