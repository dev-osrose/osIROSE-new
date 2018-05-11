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
  set(CURL_LIBRARIES "${install_dir}/${CMAKE_BUILD_TYPE}/libcurl.lib")
  set(CURL_INSTALL_LIBS "${install_dir}/${CMAKE_BUILD_TYPE}/libcurl.dll")
else()
  set(CURL_LIBRARIES "${install_dir}/lib/libcurl.so")
  set(CURL_INSTALL_LIBS "${install_dir}/lib/libcurl.so")
endif()

