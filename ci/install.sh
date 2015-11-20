#!/bin/sh

if [ -z "$TRAVIS_OS_NAME" ]; then
    TRAVIS_OS_NAME=linux
fi

if [ "$TRAVIS_OS_NAME" = "linux" ]; then
    sudo apt-get install -qq cdbs cmake libboost-dev lcov libmysqlclient-dev mysql-community-source
    gem install coveralls-lcov
    
    mkdir 3rdParty
	mkdir 3rdParty/include
	mkdir 3rdParty/lib
	
elif [ "$TRAVIS_OS_NAME" = "osx" ]; then

else
    echo "Unknown OS ($TRAVIS_OS_NAME). Stopping build ..."
    exit 1
fi
