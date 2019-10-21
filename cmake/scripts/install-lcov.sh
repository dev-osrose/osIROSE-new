#!/bin/bash
set -e
if [ -z "$TRAVIS_OS_NAME" ]; then
  TRAVIS_OS_NAME=linux
fi

echo 'Building and installing lcov.';

if [ "$TRAVIS_OS_NAME" = "linux" ]; then
  if [ ! -f "$TRAVIS_BUILD_DIR/build/3rdparty/usr/local/bin/lcov" ]; then
    echo 'Setting up lcov...';

    export DESTDIR=$TRAVIS_BUILD_DIR/build/3rdparty
    wget https://downloads.sourceforge.net/ltp/lcov-1.13.tar.gz
    tar -xf lcov-1.13.tar.gz
    rm lcov-1.13.tar.gz
    cd lcov-1.13
    make install
    cd ..
    rm -r lcov-1.13

    echo 'Finished installing lcov.';
  else
    echo 'Using cached lcov directory.';
  fi
  gem install coveralls-lcov
else
    echo "Unknown OS ($TRAVIS_OS_NAME). Stopping build ..."
    exit 1
fi
