set(CURL_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

if(WIN32)
  ExternalProject_Add(
    curl
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG curl-7_59_0
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CURL_INSTALL_DIR} -DBUILD_TESTING=OFF -DHTTP_ONLY=ON
    INSTALL_DIR ${CURL_INSTALL_DIR}
  )
else()
  ExternalProject_Add(
    curl
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG curl-7_59_0
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CURL_INSTALL_DIR} -DBUILD_TESTING=OFF -DHTTP_ONLY=ON
    INSTALL_DIR ${CURL_INSTALL_DIR}
  )
endif()

ExternalProject_Get_Property(
  curl
  install_dir
)

set(CURL_INCLUDE_DIRS "${install_dir}/include")

if(WIN32)
  set(CURL_LIBRARIES "$<$<CONFIG:Release>:${install_dir}/lib/libcurl_imp.lib>$<$<CONFIG:Debug>:${install_dir}/lib/libcurl-d_imp.lib>")
  set(CURL_INSTALL_LIBS "$<$<CONFIG:Release>:${install_dir}/bin/libcurl.dll>$<$<CONFIG:Debug>:${install_dir}/bin/libcurl-d.dll>")
else()
  set(CURL_LIBRARIES "${install_dir}/lib/libcurl.so")
  set(CURL_INSTALL_LIBS "${install_dir}/lib/libcurl.so")
endif()

