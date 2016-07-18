#!/bin/sh

sudo apt-get install cmake
sudo apt-add-repository -y "ppa:ubuntu-toolchain-r/test"
sudo apt-get update
sudo apt-get install gcc-5
sudo apt-get install g++-5
sudo apt-get install unzip
sudo apt-get install autoconf
sudo apt-get install libtool
sudo apt-get install libssl-dev
sudo apt-get install python libmysqlclient-dev libmysqld-dev libmysqlclient18 libmysql++

git submodule init
git submodule update

# Install ruby
wget https://cache.ruby-lang.org/pub/ruby/2.2/ruby-2.2.3.tar.gz
tar -xf ruby-2.2.3.tar.gz
cd ruby-2.2.3/
./configure
make && make install

cd ..

export PROOT=$(pwd)
export GCC_VERSION="5"
export GCOV_PATH=/usr/bin/gcov-${GCOV_VERSION}
export LCOV_PATH=$HOME/usr/bin/lcov
export GENHTML_PATH=$HOME/usr/bin/genhtml
export PREFIX=$HOME
export CXX="/usr/bin/g++-${GCC_VERSION}" CC="/usr/bin/gcc-${GCC_VERSION}"
export BUILD_TYPE=Debug
./ci/before_install.sh
./ci/install.sh

mkdir build
cd build
cmake -DENABLE_TESTING=OFF -DENABLE_TESTING_COVERAGE=OFF -DBUILD_PROTOBUF=OFF -DBUILD_MYSQLPP=OFF ..
