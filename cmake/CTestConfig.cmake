## This file should be placed in the root directory of your project.
## Then modify the CMakeLists.txt file in the root directory of your
## project to incorporate the testing dashboard.
## # The following are required to uses Dart and the Cdash dashboard
##   ENABLE_TESTING()
##   INCLUDE(CTest)
set(CTEST_PROJECT_NAME "osirose")

set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "ci.appveyor.com")
set(CTEST_DROP_LOCATION "/api/testresults/xunit/$ENV{APPVEYOR_JOB_ID}")
set(CTEST_DROP_SITE_CDASH FALSE)