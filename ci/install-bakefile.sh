#!/bin/bash
set -e
if [ -z "$TRAVIS_OS_NAME" ]; then
    TRAVIS_OS_NAME=linux
fi

if [ "$TRAVIS_OS_NAME" = "linux" ]; then	
  if [ ! -d "$HOME/bakefile/bin" ]; then
    # Install bakefile
    wget https://github.com/vslavik/bakefile/releases/download/v0.2.9/bakefile-0.2.9.tar.gz
    tar -xf bakefile-0.2.9.tar.gz
    cd bakefile-0.2.9/
    ./configure --prefix=$HOME/bakefile
    make -j4
    make install
    cd ..
  else
		echo 'Using cached bakefile directory.';
	fi
else
    echo "Unknown OS ($TRAVIS_OS_NAME). Stopping build ..."
    exit 1
fi
