set(CURL_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

set(_byproducts
    ${CURL_INSTALL_DIR}/lib/libcurl_imp.lib
    ${CURL_INSTALL_DIR}/lib/libcurl-d_imp.lib
    ${CURL_INSTALL_DIR}/lib/libcurl.a
    ${CURL_INSTALL_DIR}/lib/libcurl.so
  )

if(WIN32)
  ExternalProject_Add(
    curl
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG cb529b713f4882ac65a074ae8d87faa41d19168e
    CMAKE_ARGS -G ${CMAKE_GENERATOR} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${CURL_INSTALL_DIR} -DBUILD_TESTING=OFF -DHTTP_ONLY=ON
	DEPENDS ZLIB::ZLIB
    BUILD_BYPRODUCTS ${_byproducts}
    INSTALL_DIR ${CURL_INSTALL_DIR}
  )
else()
  ExternalProject_Add(
    curl
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG cb529b713f4882ac65a074ae8d87faa41d19168e
    CMAKE_ARGS -G ${CMAKE_GENERATOR} -DCMAKE_INSTALL_PREFIX=${CURL_INSTALL_DIR} -DBUILD_TESTING=OFF -DHTTP_ONLY=ON
	DEPENDS ZLIB::ZLIB
    BUILD_BYPRODUCTS ${_byproducts}
    INSTALL_DIR ${CURL_INSTALL_DIR}
  )
endif()

ExternalProject_Get_Property(
  curl
  install_dir
)

set(CURL_INCLUDE_DIR "${install_dir}/include")
if(WIN32)
  if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(CURL_LIBRARY "${install_dir}/lib/libcurl-d_imp.lib")
  elseif("${CMAKE_BUILD_TYPE}" STREQUAL "$<CONFIG>")
    set(CURL_LIBRARY "${install_dir}/lib/libcurl$<$<CONFIG:Debug>:-d>_imp.lib")
  else()
    set(CURL_LIBRARY "${install_dir}/lib/libcurl_imp.lib")
  endif()
  set(CURL_LIBRARIES "${CURL_LIBRARY}")
  set(CURL_INSTALL_LIBS "${install_dir}/bin/libcurl.dll")
else()
  set(CURL_LIBRARY "${install_dir}/lib/libcurl.so")
  set(CURL_LIBRARIES "${CURL_LIBRARY}")
  set(CURL_INSTALL_LIBS "${install_dir}/lib/libcurl.so")
endif()

if(NOT TARGET CURL::libcurl)
    add_library(CURL::libcurl INTERFACE IMPORTED)
    add_dependencies(CURL::libcurl curl)
    set_target_properties(CURL::libcurl PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CURL_INCLUDE_DIR}")
    set_target_properties(CURL::libcurl PROPERTIES INTERFACE_LINK_LIBRARIES "${CURL_LIBRARIES}")
endif()

mark_as_advanced( CURL_INCLUDE_DIR CURL_LIBRARIES CURL_LIBRARY CURL_INSTALL_LIBS )