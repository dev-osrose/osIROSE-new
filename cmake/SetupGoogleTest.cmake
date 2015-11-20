INCLUDE(Compiler)
INCLUDE(ExternalProject)

FUNCTION(SETUP_GOOGLE_TEST)
    SET_DIRECTORY_PROPERTIES(properties EP_PREFIX "${CMAKE_BINARY_DIR}/tools")
    ExternalProject_ADD(googletest
        GIT_SUBMODULES googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        SOURCE_DIR "${CMAKE_BINARY_DIR}/tools/googletest/googletest"
        CMAKE_ARGS "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}" "-DCMAKE_CXX_FLAGS=-fPIC"
        INSTALL_COMMAND ""
    )
    ExternalProject_Add_Step(googletest enter_dir
    	COMMAND mv -f ${CMAKE_BINARY_DIR}/tools/googletest/googletest ${CMAKE_BINARY_DIR}/tools/temp_move/
    	COMMAND rm -r ${CMAKE_BINARY_DIR}/tools/googletest
    	COMMAND mv -f ${CMAKE_BINARY_DIR}/tools/temp_move/ ${CMAKE_BINARY_DIR}/tools/googletest/
    	COMMENT "Fixing google test directories"
    	DEPENDEES mkdir update patch download 
    	DEPENDERS configure build install
    )
    
    ExternalProject_GET_PROPERTY(googletest SOURCE_DIR)
    ExternalProject_GET_PROPERTY(googletest BINARY_DIR)

    SET(GTEST_INCLUDE_DIR ${SOURCE_DIR}/include PARENT_SCOPE)
    SET(GTEST_BINARY_DIR ${BINARY_DIR} PARENT_SCOPE)
    
#    SET_DIRECTORY_PROPERTIES(properties EP_PREFIX "${CMAKE_BINARY_DIR}/tools")
#    ExternalProject_ADD(googlemock
#        GIT_SUBMODULES googletest
#        GIT_REPOSITORY https://github.com/google/googletest.git
#        SOURCE_DIR "${CMAKE_BINARY_DIR}/tools/googletest/googlemock"
#        CMAKE_ARGS "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}" "-DCMAKE_CXX_FLAGS=-fPIC"
#        INSTALL_COMMAND ""
#    )
#    ExternalProject_Add_Step(googlemock enter_dir
#    	COMMAND mv -f ${CMAKE_BINARY_DIR}/tools/googletest/googletest ${CMAKE_BINARY_DIR}/tools/temp_move/
#    	COMMAND rm -r ${CMAKE_BINARY_DIR}/tools/googletest
#    	COMMAND mv -f ${CMAKE_BINARY_DIR}/tools/temp_move/ ${CMAKE_BINARY_DIR}/tools/googletest/
#    	COMMENT "Fixing google test directories"
#    	DEPENDEES mkdir update patch download 
#    	DEPENDERS configure build install
#    )
#    
#    ExternalProject_GET_PROPERTY(googlemock SOURCE_DIR)
#    ExternalProject_GET_PROPERTY(googlemock BINARY_DIR)

#    SET(GMOCK_INCLUDE_DIR ${SOURCE_DIR}/include PARENT_SCOPE)
#    SET(GMOCK_BINARY_DIR ${BINARY_DIR} PARENT_SCOPE)
   
ENDFUNCTION()
