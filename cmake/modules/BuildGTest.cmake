
if(MSVC11)
  set(ADD_CXX_FLAGS "/D_VARIADIC_MAX=10")
endif()

# This is a workaround for Ninja not allowing us to build if these libs weren't built before
if(WIN32)
  ExternalProject_Add(
    gtest
    GIT_SUBMODULES googletest...
    SOURCE_DIR ${CMAKE_THIRD_PARTY_DIR}/googletest/googletest
    CMAKE_ARGS -DBUILD_SHARED_LIBS=ON -DCMAKE_CXX_FLAGS=${ADD_CXX_FLAGS} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -Dgtest_force_shared_crt=ON
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND ""
  )
else()
  set(_byproducts
    ${CMAKE_THIRD_PARTY_DIR}/googletest/googletest/libgtest.so
  )
  
  ExternalProject_Add(
    gtest
    GIT_SUBMODULES googletest...
    SOURCE_DIR ${CMAKE_THIRD_PARTY_DIR}/googletest/googletest
    CMAKE_ARGS -DBUILD_SHARED_LIBS=ON -DCMAKE_CXX_FLAGS=${ADD_CXX_FLAGS} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -Dgtest_force_shared_crt=ON
    BUILD_BYPRODUCTS ${_byproducts}
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND ""
  )
endif()



ExternalProject_Get_Property(
  gtest
  source_dir
)

set(GTEST_INCLUDE_DIRS "${source_dir}/include")

if(WIN32)
  set(GTEST_LIBRARIES "${source_dir}/${CMAKE_BUILD_TYPE}/gtest.lib")
  set(GTEST_INSTALL_LIBS "${source_dir}/${CMAKE_BUILD_TYPE}/gtest.dll")
else()
  set(GTEST_LIBRARIES "${source_dir}/libgtest.so")
  set(GTEST_INSTALL_LIBS "${source_dir}/libgtest.so")
endif()

set(GTEST_BOTH_LIBRARIES ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})

install_external_library(gtest ${GTEST_INSTALL_LIBS})
