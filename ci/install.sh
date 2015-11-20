#!/bin/sh

if [ -z "$TRAVIS_OS_NAME" ]; then
    TRAVIS_OS_NAME=linux
fi

if [ "$TRAVIS_OS_NAME" = "linux" ]; then

	wget http://downloads.sourceforge.net/ltp/lcov-1.12.tar.gz
	tar -xf lcov-1.12.tar.gz
	cd lcov-1.12
	sudo make install

	cd ..
	sudo apt-get install -qq cdbs cmake libboost-dev libmysqlclient-dev
	gem install coveralls-lcov
    
	mkdir 3rdParty
	mkdir 3rdParty/include
	mkdir 3rdParty/lib
else
    echo "Unknown OS ($TRAVIS_OS_NAME). Stopping build ..."
    exit 1
fi
