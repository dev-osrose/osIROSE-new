if(WIN32)
  ExternalProject_Add(
    curl
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG curl-7_59_0
    CMAKE_ARGS -DBUILD_TESTING=OFF -DHTTP_ONLY=ON
    BUILD_IN_SOURCE 1
  )
else()
  ExternalProject_Add(
    curl
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG curl-7_59_0
    CMAKE_ARGS -DBUILD_TESTING=OFF -DHTTP_ONLY=ON
    BUILD_IN_SOURCE 1
  )
endif()

ExternalProject_Get_Property(
  curl
  source_dir
)

set(CURL_INCLUDE_DIRS "${source_dir}/include")

if(WIN32)
  set(CURL_LIBRARIES "${source_dir}/${CMAKE_BUILD_TYPE}/libcurl.lib")
  set(CURL_INSTALL_LIBS "${source_dir}/${CMAKE_BUILD_TYPE}/libcurl.dll")
else()
  set(CURL_LIBRARIES "${source_dir}/libcurl.so")
  set(CURL_INSTALL_LIBS "${source_dir}/libcurl.so")
endif()

install_external_library(curl ${CURL_INSTALL_LIBS})

