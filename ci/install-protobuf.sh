#!/bin/bash
set -e
if [ -z "$TRAVIS_OS_NAME" ]; then
    TRAVIS_OS_NAME=linux
fi

echo 'Building and installing protobuf.';

if [ "$TRAVIS_OS_NAME" = "linux" ]; then
	if [[ ! -d "$HOME/protobuf/lib" || ! -f "$HOME/protobuf/bin/protoc" ]]; then
		echo 'Setting up protobuf...';
		cd 3rdparty/protobuf
		./autogen.sh
		./configure --prefix=$HOME/protobuf
		make -j 4 && make install

		cd ../../
	else
		echo 'Using cached protobuf directory.';
	fi
else
    echo "Unknown OS ($TRAVIS_OS_NAME). Stopping build ..."
    exit 1
fi
