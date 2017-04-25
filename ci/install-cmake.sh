#!/bin/bash
set -e
if [ -z "$TRAVIS_OS_NAME" ]; then
    TRAVIS_OS_NAME=linux
fi

echo 'Building and installing cmake.';

if [[ "${TRAVIS_OS_NAME}" == "linux" ]]; then
  CMAKE_URL="https://cmake.org/files/v3.7/cmake-3.7.2-Linux-x86_64.tar.gz"
  mkdir cmake_app && travis_retry wget --quiet -O - ${CMAKE_URL} | tar --strip-components=1 -xz -C cmake_app
  export PATH=${TRAVIS_BUILD_DIR}/cmake_app/bin:${PATH}
else
  brew install cmake
fi
