#!/bin/sh

apt-get install cmake
apt-add-repository -y "ppa:ubuntu-toolchain-r/test"
apt-get update
apt-get install gcc-5
apt-get install g++-5
apt-get install unzip
apt-get install autoconf
apt-get install libtool
apt-get install libssl-dev

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
cmake ..
