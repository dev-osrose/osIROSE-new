set(ZLIB_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

set(_byproducts
  ${ZLIB_INSTALL_DIR}/zlib.lib
  ${ZLIB_INSTALL_DIR}/libzlib.a
)

#if(WIN32 OR MINGW)
#  if(MINGW)
#    ExternalProject_Add(
#      zlib
#      URL https://www.zlib.net/zlib1211.zip
#	  URL_HASH SHA256=d7510a8ee1918b7d0cad197a089c0a2cd4d6df05fee22389f67f115e738b178d
#      #CONFIGURE_COMMAND ""
#      #BUILD_COMMAND gcc -shared 
#      #<SOURCE_DIR>/adler32.c
#      #<SOURCE_DIR>/compress.c
#      #<SOURCE_DIR>/crc32.c
#      #<SOURCE_DIR>/deflate.c
#      #<SOURCE_DIR>/gzio.c
#      #<SOURCE_DIR>/inffast.c
#      #<SOURCE_DIR>/inflate.c
#      #<SOURCE_DIR>/inftrees.c
#      #<SOURCE_DIR>/trees.c
#      #<SOURCE_DIR>/uncompr.c
#      #<SOURCE_DIR>/zutil.c -o ${ZLIB_INSTALL_DIR}/zlib.a
#      #BUILD_IN_SOURCE true
#      BUILD_BYPRODUCTS ${_byproducts}
#      #INSTALL_COMMAND ""
#      INSTALL_DIR ${ZLIB_INSTALL_DIR}
#    )
#  else()
    ExternalProject_Add(
      zlib
      URL https://www.zlib.net/zlib1211.zip
	  URL_HASH SHA256=d7510a8ee1918b7d0cad197a089c0a2cd4d6df05fee22389f67f115e738b178d
      CMAKE_ARGS -G${CMAKE_GENERATOR} -DCMAKE_INSTALL_PREFIX=${ZLIB_INSTALL_DIR}
      BUILD_BYPRODUCTS ${_byproducts}
      INSTALL_DIR ${ZLIB_INSTALL_DIR}
    )
#  endif()
#endif()


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