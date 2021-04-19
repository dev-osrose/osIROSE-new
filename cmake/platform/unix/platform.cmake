set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)
set(CMAKE_INSTALL_RPATH "./")

set(CPACK_GENERATOR "DEB" "TGZ")
set(CPACK_PACKAGE_VENDOR "osrose")
set(CPACK_PACKAGE_VERSION_MAJOR 0)
set(CPACK_PACKAGE_VERSION_MINOR 3)
set(CPACK_PACKAGE_VERSION_PATCH 1)

set(CPACK_DEBIAN_FILE_NAME "DEB-DEFAULT")
set(CPACK_DEBIAN_PACKAGE_SECTION "games")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "RavenX8#3180")

add_compile_definitions(ASIO_STANDALONE)
add_compile_options(-Wall -Wextra)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  add_compile_definitions(_DEBUG DEBUG)
  message(STATUS "Configured for Debug")
else()
  add_compile_definitions(NDEBUG)
  message(STATUS "Configured for Release")
endif()

set(COPY_SCRIPT_PATH "")
set(COPY_FLAT_SCRIPT_PATH "")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(64BIT TRUE)
  message("-- detected 64bit")
else()
  set(64BIT FALSE)
  message("-- detected 32bit")
endif()