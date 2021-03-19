set(OSIROSE_COMMON_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

set(_byproducts
    ${OSIROSE_COMMON_INSTALL_DIR}/lib/osIROSE-common-core.lib
    ${OSIROSE_COMMON_INSTALL_DIR}/lib/osIROSE-common-rosecommon.lib
    ${OSIROSE_COMMON_INSTALL_DIR}/lib/libosIROSE-common-core.a
    ${OSIROSE_COMMON_INSTALL_DIR}/lib/libosIROSE-common-rosecommon.a
  )

ExternalProject_Add(
    OSIROSE_COMMON
    GIT_REPOSITORY https://github.com/dev-osrose/packets
    CMAKE_ARGS -G ${CMAKE_GENERATOR} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${OSIROSE_COMMON_INSTALL_DIR} -DENABLE_TESTS=OFF
    INSTALL_DIR ${OSIROSE_COMMON_INSTALL_DIR}
    BUILD_BYPRODUCTS ${_byproducts}
)

ExternalProject_Get_Property(
    OSIROSE_COMMON
    install_dir
)

if(WIN32)
    set(OSIROSE_COMMON_LIBRARIES "${install_dir}/lib/osIROSE-common-core.lib" "${install_dir}/lib/osIROSE-common-rosecommon.lib")
else()
    set(OSIROSE_COMMON_LIBRARIES "${install_dir}/lib/libosIROSE-common-core.a" "${install_dir}/lib/libosIROSE-common-rosecommon.a")
endif()

set(OSIROSE_COMMON_INCLUDE_DIR "${install_dir}/include")
if(NOT TARGET osirose_common::osirose_common)
    add_library(osirose_common::osirose_common INTERFACE IMPORTED)
    add_dependencies(osirose_common::osirose_common OSIROSE_COMMON osIROSE-common-core osIROSE-common-rosecommon)
    set_target_properties(osirose_common::osirose_common PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${OSIROSE_COMMON_INCLUDE_DIR}")
    set_target_properties(osirose_common::osirose_common PROPERTIES INTERFACE_LINK_LIBRARIES "${OSIROSE_COMMON_LIBRARIES}")
endif()
