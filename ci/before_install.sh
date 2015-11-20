#!/bin/sh

if [ -z "$TRAVIS_OS_NAME" ]; then
    export TRAVIS_OS_NAME=linux
fi

if [ "$TRAVIS_OS_NAME" = "linux" ]; then
	export MAIN_PATH=$(pwd);
	sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test;
	sudo add-apt-repository -y "deb http://mirrors.kernel.org/ubuntu/ trusty main";
	sudo add-apt-repository -y "deb http://repo.mysql.com/apt/ubuntu/ precise mysql-5.7";
    sudo apt-get update -qq;
    git submodule update --init --recursive;
    
    export COVERALLS_SERVICE_NAME=travis-ci;
    
    if [ "$CXX" = "g++" ]; then 
    	sudo apt-get install -qq "g++-${GCC_VERSION}";
    	export CXX="g++-${GCC_VERSION}" CC="gcc-${GCC_VERSION}" ;
    fi
    
	if [ "$CXX" = "clang++" ]; then 
		sudo apt-get install -qq "clang-3.4";
		export CXX="clang++-3.4" CC="clang-3.4" ;
	fi

elif [ "$TRAVIS_OS_NAME" = "osx" ]; then
    brew update && brew --config
else
    echo "Unknown OS - '$TRAVIS_OS_NAME'. Stopping the build ..."
    exit 1
fi    
