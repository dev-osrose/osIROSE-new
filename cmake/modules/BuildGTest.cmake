set(GTEST_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

if(WIN32)
  set(GTEST_ADD_CXX_FLAGS "/w")
endif()

if(NOT BUILD_SHARED_LIBS)
  set(BUILD_SHARED_LIBS OFF)
endif()

if(NOT GMOCK_FOUND AND WITH_GMOCK)
  set(BUILD_GMOCK ON)
else()
  set(BUILD_GMOCK OFF)
endif()

if(WIN32 AND NOT MINGW)
  set(_byproducts)
  if(NOT MSBUILD)
    if(${BUILD_SHARED_LIBS})
      set(_byproducts
        ${GTEST_INSTALL_DIR}/lib/gtestd.lib
        ${GTEST_INSTALL_DIR}/lib/gmockd.lib
        
        ${GTEST_INSTALL_DIR}/lib/gtest.lib
        ${GTEST_INSTALL_DIR}/lib/gmock.lib
        
        ${GTEST_INSTALL_DIR}/bin/gtestd.dll
        ${GTEST_INSTALL_DIR}/bin/gmockd.dll
        
        ${GTEST_INSTALL_DIR}/bin/gtest.dll
        ${GTEST_INSTALL_DIR}/bin/gmock.dll
      )
    else()
      set(_byproducts
        ${GTEST_INSTALL_DIR}/lib/gtestd.lib
        ${GTEST_INSTALL_DIR}/lib/gmockd.lib
        
        ${GTEST_INSTALL_DIR}/lib/gtest.lib
        ${GTEST_INSTALL_DIR}/lib/gmock.lib
      )
    endif()
  endif()
  
  ExternalProject_Add(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG 4e4df226fc197c0dda6e37f5c8c3845ca1e73a49

    CMAKE_ARGS -G ${CMAKE_GENERATOR} -DCMAKE_INSTALL_PREFIX=${GTEST_INSTALL_DIR} -DCMAKE_CXX_FLAGS=${GTEST_ADD_CXX_FLAGS} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DBUILD_GMOCK=${BUILD_GMOCK} -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS} -Dgmock_build_tests=OFF -Dgtest_force_shared_crt=ON
    INSTALL_DIR ${GTEST_INSTALL_DIR}
    BUILD_BYPRODUCTS ${_byproducts}
  )
else()
  # This is a workaround for Ninja not allowing us to build if these libs weren't built before
  if(${BUILD_SHARED_LIBS})
    set(_byproducts
      ${GTEST_INSTALL_DIR}/lib/libgtest.so
      ${GTEST_INSTALL_DIR}/lib/libgmock.so
    )
  else()
    set(_byproducts
      ${GTEST_INSTALL_DIR}/lib/libgtest.a
      ${GTEST_INSTALL_DIR}/lib/libgmock.a
    )
  endif()

  ExternalProject_Add(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.8.0
    GIT_SHALLOW true

    CMAKE_ARGS -G ${CMAKE_GENERATOR} -DCMAKE_INSTALL_PREFIX=${GTEST_INSTALL_DIR} -DCMAKE_CXX_FLAGS=${GTEST_ADD_CXX_FLAGS} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DBUILD_GMOCK=${BUILD_GMOCK} -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS} -Dgmock_build_tests=OFF
    BUILD_BYPRODUCTS ${_byproducts}
    INSTALL_DIR ${GTEST_INSTALL_DIR}
  )
endif()


ExternalProject_Get_Property(
  gtest
  install_dir
)

if(WIN32)
  set(GTEST_LIBRARIES "$<$<CONFIG:Release>:${install_dir}/lib/gtest.lib>$<$<CONFIG:Debug>:${install_dir}/lib/gtestd.lib>")
  set(GTEST_INSTALL_LIBS "${install_dir}/bin/gtest.dll")
  if(BUILD_GMOCK)
    set(GTEST_LIBRARIES "$<$<CONFIG:Release>:${install_dir}/lib/gmock.lib>$<$<CONFIG:Debug>:${install_dir}/lib/gmockd.lib>;${GTEST_LIBRARIES}")
    set(GTEST_INSTALL_LIBS "${install_dir}/bin/gmock.dll;${GTEST_INSTALL_LIBS}")
  endif()
else()
  if(${BUILD_SHARED_LIBS})
    set(GTEST_LIBRARIES "${install_dir}/lib/libgtest.so")
    set(GTEST_INSTALL_LIBS "${install_dir}/lib/libgtest.so")
    if(BUILD_GMOCK)
      set(GTEST_LIBRARIES "${install_dir}/lib/libgmock.so" ${GTEST_LIBRARIES})
      set(GTEST_INSTALL_LIBS "${install_dir}/lib/libgmock.so" ${GTEST_INSTALL_LIBS})
    endif()
  else()
    set(GTEST_LIBRARIES "${install_dir}/lib/libgtest.a")
    set(GTEST_INSTALL_LIBS "${install_dir}/lib/libgtest.a")
    if(BUILD_GMOCK)
      set(GTEST_LIBRARIES ${GTEST_LIBRARIES} "${install_dir}/lib/libgmock.a")
      set(GTEST_INSTALL_LIBS ${GTEST_INSTALL_LIBS} "${install_dir}/lib/libgmock.a")
    endif()
  endif()
endif()

set(GTEST_INCLUDE_DIRS "${install_dir}/include")
set(GTEST_BOTH_LIBRARIES ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})

if(NOT TARGET GTest::GTest)
    add_library(GTest::GTest INTERFACE IMPORTED)
    add_dependencies(GTest::GTest gtest)
    set_target_properties(GTest::GTest PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${GTEST_INCLUDE_DIRS}")
    set_target_properties(GTest::GTest PROPERTIES INTERFACE_LINK_LIBRARIES "${GTEST_LIBRARIES}")
endif()
