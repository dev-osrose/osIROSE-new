#!/bin/bash
set -e
if [ -z "$TRAVIS_OS_NAME" ]; then
    TRAVIS_OS_NAME=linux
fi

if [ "$TRAVIS_OS_NAME" = "linux" ]; then	
	mkdir 3rdparty
	mkdir 3rdparty/include
	mkdir 3rdparty/lib
  
  ./install-protobuf.sh
  ./install-ninja.sh
  ./install-lcov.sh
  ./install-bakefile.sh
  ./install-mysqlpp.sh
else
    echo "Unknown OS ($TRAVIS_OS_NAME). Stopping build ..."
    exit 1
fi
