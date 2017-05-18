#!/bin/bash
set -e
if [ -z "$TRAVIS_OS_NAME" ]; then
    TRAVIS_OS_NAME=linux
fi

if [ "$TRAVIS_OS_NAME" = "linux" ]; then
	cd $PROOT/3rdparty/mysqlpp
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
