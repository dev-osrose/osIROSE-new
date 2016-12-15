#!/bin/sh

if [ -z "$TRAVIS_OS_NAME" ]; then
  export TRAVIS_OS_NAME=linux
fi

if [ "$TRAVIS_OS_NAME" = "linux" ]; then
  mkdir build && cd build
  mkdir logs
else
  echo "Unknown OS - '$TRAVIS_OS_NAME'. Stopping the build ..."
  exit 1
fi    
