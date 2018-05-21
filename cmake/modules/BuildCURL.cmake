set(CURL_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

if(WIN32)
  ExternalProject_Add(
    curl
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG cb529b713f4882ac65a074ae8d87faa41d19168e
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CURL_INSTALL_DIR} -DBUILD_TESTING=OFF -DHTTP_ONLY=ON
    INSTALL_DIR ${CURL_INSTALL_DIR}
  )
else()
  set(_byproducts
    ${CURL_INSTALL_DIR}/lib/libcurl.a
    ${CURL_INSTALL_DIR}/lib/libcurl.so
  )
  ExternalProject_Add(
    curl
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG cb529b713f4882ac65a074ae8d87faa41d19168e
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CURL_INSTALL_DIR} -DBUILD_TESTING=OFF -DHTTP_ONLY=ON
    BUILD_BYPRODUCTS ${_byproducts}
    INSTALL_DIR ${CURL_INSTALL_DIR}
  )
endif()

ExternalProject_Get_Property(
  curl
  install_dir
)

set(CURL_INCLUDE_DIRS "${install_dir}/include")

if(WIN32)
  set(CURL_LIBRARY "$<$<CONFIG:Release>:${install_dir}/lib/libcurl_imp.lib>$<$<CONFIG:Debug>:${install_dir}/lib/libcurl-d_imp.lib>")
  set(CURL_LIBRARIES "${CURL_LIBRARY}")
  set(CURL_INSTALL_LIBS "$<$<CONFIG:Release>:${install_dir}/bin/libcurl.dll>$<$<CONFIG:Debug>:${install_dir}/bin/libcurl-d.dll>")
else()
  set(CURL_LIBRARY "${install_dir}/lib/libcurl.so")
  set(CURL_LIBRARIES "${CURL_LIBRARY}")
  set(CURL_INSTALL_LIBS "${install_dir}/lib/libcurl.so")
endif()

if(NOT TARGET CURL::CURL)
    add_library(CURL::CURL UNKNOWN IMPORTED)
    set_target_properties(CURL::CURL PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CURL_INCLUDE_DIRS}")
    set_property(TARGET CURL::CURL APPEND PROPERTY IMPORTED_LOCATION "${CURL_LIBRARY}")
endif()