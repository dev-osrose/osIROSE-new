#!/bin/sh

if [ -z "$TRAVIS_OS_NAME" ]; then
    export TRAVIS_OS_NAME=linux
fi

if [ "$TRAVIS_OS_NAME" = "linux" ]; then
	export MAIN_PATH=$(pwd);
	git submodule update --init --recursive;
    
	export COVERALLS_SERVICE_NAME=travis-ci;
    
	if [ "$CXX" = "g++" ]; then 
		export CXX="/usr/bin/g++-${GCC_VERSION}" CC="/usr/bin/gcc-${GCC_VERSION}";
		gcc --version
	elif [ "$CXX" = "clang++" ]; then 
		export CXX="/usr/bin/clang++-3.7" CC="/usr/bin/clang-3.7";
	fi
elif [ "$TRAVIS_OS_NAME" = "osx" ]; then
    brew update && brew --config
else
    echo "Unknown OS - '$TRAVIS_OS_NAME'. Stopping the build ..."
    exit 1
fi    
