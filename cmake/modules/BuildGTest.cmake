
if(MSVC11)
  set(ADD_CXX_FLAGS "/D_VARIADIC_MAX=10")
endif()

if(NOT BUILD_SHARED_LIBS)
  set(BUILD_SHARED_LIBS OFF)
endif()

if(NOT GMOCK_FOUND AND WITH_GMOCK)
  set(BUILD_GMOCK ON)
else()
  set(BUILD_GMOCK OFF)
endif()

if(WIN32)
  ExternalProject_Add(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG 278aba369c41e90e9e77a6f51443beb3692919cf
    GIT_SHALLOW true
    
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=$ENV{PREFIX} -DCMAKE_CXX_FLAGS=${ADD_CXX_FLAGS} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DBUILD_GMOCK=${BUILD_GMOCK} -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS} -Dgmock_build_tests=OFF
    INSTALL_DIR $ENV{PREFIX}
  )
else()
  # This is a workaround for Ninja not allowing us to build if these libs weren't built before
  if(${BUILD_SHARED_LIBS})
    set(_byproducts
      $ENV{PREFIX}/lib/libgtest.a
      $ENV{PREFIX}/lib/libgmock.a
    )
  else()
    set(_byproducts
      $ENV{PREFIX}/lib/libgtest.so
      $ENV{PREFIX}/lib/libgmock.so
    )
  endif()
  
  ExternalProject_Add(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG 278aba369c41e90e9e77a6f51443beb3692919cf
    GIT_SHALLOW true
    
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=$ENV{PREFIX} -DCMAKE_CXX_FLAGS=${ADD_CXX_FLAGS} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DBUILD_GMOCK=${BUILD_GMOCK} -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS} -Dgmock_build_tests=OFF
    BUILD_BYPRODUCTS ${_byproducts}
    INSTALL_DIR $ENV{PREFIX}
  )
endif()


ExternalProject_Get_Property(
  gtest
  install_dir
)

set(GTEST_INCLUDE_DIRS "${install_dir}/include/gtest")
if(BUILD_GMOCK)
  set(GTEST_INCLUDE_DIRS "${install_dir}/include/gmock;${GTEST_INCLUDE_DIRS}")
endif()

if(WIN32)
  set(GTEST_LIBRARIES "${install_dir}/${CMAKE_BUILD_TYPE}/gtest.lib")
  set(GTEST_INSTALL_LIBS "${install_dir}/${CMAKE_BUILD_TYPE}/gtest.dll")
else()
  if(${BUILD_SHARED_LIBS})
    set(GTEST_LIBRARIES "${install_dir}/lib/libgtest.so")
    set(GTEST_INSTALL_LIBS "${install_dir}/lib/libgtest.so")
  else()
    set(GTEST_LIBRARIES "${install_dir}/lib/libgtest.a")
    set(GTEST_INSTALL_LIBS "${install_dir}/lib/libgtest.a")
  endif()
endif()

set(GTEST_BOTH_LIBRARIES ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
