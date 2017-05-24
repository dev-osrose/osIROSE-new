#!/bin/bash
set -e
if [ -z "$TRAVIS_OS_NAME" ]; then
    TRAVIS_OS_NAME=linux
fi

echo 'Building and installing mysql.';

if [ "$TRAVIS_OS_NAME" = "linux" ]; then
    pip install --user pyparsing
	if [ ! -d "$HOME/mysql/lib" ]; then
		echo 'Setting up mysql...';
    wget http://dev.mysql.com/get/Downloads/Connector-C/mysql-connector-c-6.1.6-src.tar.gz
    tar -xf mysql-connector-c-6.1.6-src.tar.gz
    rm mysql-connector-c-6.1.6-src.tar.gz
    cd mysql-connector-c-6.1.6-src
    cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX=$HOME/mysql
		make
		make install
		cd ..
    rm -r mysql-connector-c-6.1.6-src
    echo 'Finished installing mysql.';
	else
		echo 'Using cached mysql directory.';
	fi
else
    echo "Unknown OS ($TRAVIS_OS_NAME). Stopping build ..."
    exit 1
fi
