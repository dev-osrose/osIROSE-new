include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
  GIT_SHALLOW true
)

# We only need gtest; gmock is not used by any test in this project.
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
# Windows: link the same CRT as the rest of the build.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# googletest already provides GTest::gtest / GTest::gtest_main. Older code in
# this project linked GTest::GTest, so keep that name working.
if(NOT TARGET GTest::GTest)
  add_library(GTest::GTest ALIAS gtest)
endif()

set(GTEST_INCLUDE_DIRS "${googletest_SOURCE_DIR}/googletest/include")
set(GTEST_LIBRARIES GTest::gtest)
set(GTEST_BOTH_LIBRARIES GTest::gtest GTest::gtest_main)
