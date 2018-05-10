#!/bin/sh

if [ -z "$TRAVIS_OS_NAME" ]; then
    export TRAVIS_OS_NAME=linux
fi

if [ -z "$GCC_VERSION" ]; then
    export GCC_VERSION=7
fi

if [ -z "$CLANG_VERSION" ]; then
    export CLANG_VERSION=5.0
fi

if [ "$TRAVIS_OS_NAME" = "linux" ]; then
	export MAIN_PATH=$(pwd);
	git submodule update --init --recursive;

	export COVERALLS_SERVICE_NAME=travis-ci;

	if [ "$CXX" = "g++" ]; then
		export CXX="/usr/bin/g++-${GCC_VERSION}" CC="/usr/bin/gcc-${GCC_VERSION}";
		gcc --version
	elif [ "$CXX" = "clang++" ]; then
		export CXX="/usr/bin/clang++-${CLANG_VERSION}" CC="/usr/bin/clang-${CLANG_VERSION}";
	fi
elif [ "$TRAVIS_OS_NAME" = "osx" ]; then
    brew update && brew --config
else
    echo "Unknown OS - '$TRAVIS_OS_NAME'. Stopping the build ..."
    exit 1
fi
