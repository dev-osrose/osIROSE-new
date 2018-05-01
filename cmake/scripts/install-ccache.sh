#!/bin/bash
set -e
if [ -z "$TRAVIS_OS_NAME" ]; then
    TRAVIS_OS_NAME=linux
fi

echo 'Building and installing ccache.';

if [ "$TRAVIS_OS_NAME" = "linux" ]; then
	if [ ! -f "$TRAVIS_BUILD_DIR/3rdparty/bin/ccache" ]; then
		echo 'Setting up ccache...';
    
    wget https://www.samba.org/ftp/ccache/ccache-3.4.2.tar.gz
    tar -xf ccache-3.4.2.tar.gz
    rm ccache-3.4.2.tar.gz
    cd ccache-3.4.2
    ./configure --prefix=$TRAVIS_BUILD_DIR/3rdparty
    make
    make install
    cd .. && rm -r ccache-3.4.2
    
    echo 'Finished installing ccache.';
	else
		echo 'Using cached ccache directory.';
	fi
else
    echo "Unknown OS ($TRAVIS_OS_NAME). Stopping build ..."
    exit 1
fi
