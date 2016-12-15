#!/bin/bash

if [ -z "$TRAVIS_OS_NAME" ]; then
    TRAVIS_OS_NAME=linux
fi

if [ "$TRAVIS_OS_NAME" = "linux" ]; then	
	git clone https://github.com/ninja-build/ninja.git
	cd ninja
	./configure.py --bootstrap
	cd $PROOT

	wget http://downloads.sourceforge.net/ltp/lcov-1.12.tar.gz
	tar -xf lcov-1.12.tar.gz
	cd lcov-1.12
	make install

	cd $PROOT
	gem install coveralls-lcov
    
	mkdir 3rdparty
	mkdir 3rdparty/include
	mkdir 3rdparty/lib

# Install bakefile
	wget https://github.com/vslavik/bakefile/releases/download/v0.2.9/bakefile-0.2.9.tar.gz
	tar -xf bakefile-0.2.9.tar.gz
	cd bakefile-0.2.9/
	./configure --prefix=$HOME/bakefile
	make -j4
	make install
	
	cd $PROOT/tools/mysqlpp
  export PATH=$PATH:$HOME/bakefile
  echo $PATH
	./bootstrap
	./configure

	cd lib
	perl querydef.pl
	perl ssqls.pl
	cd $PROOT
else
    echo "Unknown OS ($TRAVIS_OS_NAME). Stopping build ..."
    exit 1
fi
