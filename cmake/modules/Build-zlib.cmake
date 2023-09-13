set(ZLIB_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

set(_byproducts
  ${ZLIB_INSTALL_DIR}/zlib.lib
  ${ZLIB_INSTALL_DIR}/libzlib.a
)

ExternalProject_Add(
  zlib
  URL https://www.zlib.net/zlib13.zip
  URL_HASH SHA256=c561d09347f674f0d72692e7c75d9898919326c532aab7f8c07bb43b07efeb38
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  CMAKE_ARGS -G${CMAKE_GENERATOR} -DCMAKE_INSTALL_PREFIX=${ZLIB_INSTALL_DIR}
  BUILD_BYPRODUCTS ${_byproducts}
  INSTALL_DIR ${ZLIB_INSTALL_DIR}
)

ExternalProject_Get_Property(
  zlib
  source_dir
  install_dir
)

set(ZLIB_INCLUDE_DIRS "${source_dir}")
if(NOT MINGW AND WIN32)
  set(ZLIB_LIBRARY_DIR "${install_dir}")
  set(ZLIB_LIBRARY "${install_dir}/zlib.lib")
  set(ZLIB_LIBRARIES "${install_dir}/zlib.lib" "${ZLIB_LIBRARIES}")
else()
  set(ZLIB_LIBRARY_DIR "${install_dir}")
  set(ZLIB_LIBRARY "${install_dir}/zlib.a")
  set(ZLIB_LIBRARIES "${install_dir}/zlib.a" "${ZLIB_LIBRARIES}")
endif()

if(NOT TARGET ZLIB::ZLIB)
  add_library(ZLIB::ZLIB INTERFACE IMPORTED)
  add_dependencies(ZLIB::ZLIB zlib)
  set_target_properties(ZLIB::ZLIB PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${ZLIB_INCLUDE_DIR}")
  set_target_properties(ZLIB::ZLIB PROPERTIES INTERFACE_LINK_LIBRARIES "${ZLIB_LIBRARIES}")
endif()

mark_as_advanced( ZLIB_INCLUDE_DIRS ZLIB_LIBRARIES ZLIB_LIBRARY ZLIB_LIBRARY_PATH )
