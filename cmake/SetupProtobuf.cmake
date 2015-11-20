INCLUDE(Compiler)
INCLUDE(ExternalProject)

FUNCTION(SETUP_PROTOBUF)
    SET_DIRECTORY_PROPERTIES(properties EP_PREFIX "${CMAKE_BINARY_DIR}/tools")
    ExternalProject_ADD(protobuf
        GIT_SUBMODULES protobuf
        GIT_REPOSITORY https://github.com/google/protobuf.git
        SOURCE_DIR "${CMAKE_BINARY_DIR}/tools/protobuf"
        CMAKE_ARGS "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}" "-DCMAKE_CXX_FLAGS=-fPIC"
        INSTALL_COMMAND ""
    )
    ExternalProject_GET_PROPERTY(protobuf SOURCE_DIR)
    ExternalProject_GET_PROPERTY(protobuf BINARY_DIR)

    SET(PROTOBUF_INCLUDE_DIR ${SOURCE_DIR}/gtest/include PARENT_SCOPE)
    SET(PROTOBUF_BINARY_DIR ${BINARY_DIR}/gtest PARENT_SCOPE)
ENDFUNCTION()
