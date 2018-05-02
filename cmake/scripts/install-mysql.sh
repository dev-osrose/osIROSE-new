#!/bin/bash
set -e
if [ -z "$TRAVIS_OS_NAME" ]; then
    TRAVIS_OS_NAME=linux
fi

echo 'Building and installing mysql.';

if [ "$TRAVIS_OS_NAME" = "linux" ]; then
    pip install --user pyparsing
	if [ ! -f "$TRAVIS_BUILD_DIR/3rdparty/lib/libmysqlclient.so" ]; then
		echo 'Setting up mysql...';
    
        wget http://dev.mysql.com/get/Downloads/Connector-C/mysql-connector-c-6.1.6-src.tar.gz
        tar -xf mysql-connector-c-6.1.6-src.tar.gz
        rm mysql-connector-c-6.1.6-src.tar.gz
        cd mysql-connector-c-6.1.6-src
        if [ "x$DESTDIR" == "x" ]; then
          #The DESTDIR env var isn't set.
          cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX=$TRAVIS_BUILD_DIR/3rdparty
        else
          cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX=""
        fi
        make
        make install 
        cd .. && rm -r mysql-connector-c-6.1.6-src
    
        echo 'Finished installing mysql.';
	else
		echo 'Using cached mysql directory.';
	fi
else
    echo "Unknown OS ($TRAVIS_OS_NAME). Stopping build ..."
    exit 1
fi
