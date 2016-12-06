#!/bin/bash

if [ -z "$TRAVIS_OS_NAME" ]; then
    TRAVIS_OS_NAME=linux
fi

if [ "$TRAVIS_OS_NAME" = "linux" ]; then
	if [[ ! -d "./protobuf/lib" || ! -f "./protobuf/bin/protoc" ]]; then
		echo 'Setting up protobuf...';
		cd tools/protobuf
		./autogen.sh
		./configure --prefix=$PROOT/protobuf > /dev/null
		make -j 4
		make install

		cd ../../
	else
		echo 'Using cached protobuf directory.';
	fi
	
	git clone https://github.com/ninja-build/ninja.git
	cd ninja
	./configure.py --bootstrap
	#setenv PATH $PATH:`pwd`
	cd ..

	wget http://downloads.sourceforge.net/ltp/lcov-1.12.tar.gz
	tar -xf lcov-1.12.tar.gz
	cd lcov-1.12
	make install

	cd ..
	gem install coveralls-lcov
    
	mkdir 3rdparty
	mkdir 3rdparty/include
	mkdir 3rdparty/lib

# Install bakefile
	wget https://github.com/vslavik/bakefile/releases/download/v0.2.9/bakefile-0.2.9.tar.gz
	tar -xf bakefile-0.2.9.tar.gz
	cd bakefile-0.2.9/
	./configure --prefix=$PROOT/bakefile > /dev/null
	make -j4
	export PATH=$PATH:$PROOT/bakefile/bin
	export PATH=$PATH:$PROOT/bakefile/lib
	make install
	
	cd ../tools/mysqlpp
	./bootstrap
	./configure

	cd lib
	perl querydef.pl
	perl ssqls.pl
	cd ../../..
else
    echo "Unknown OS ($TRAVIS_OS_NAME). Stopping build ..."
    exit 1
fi
