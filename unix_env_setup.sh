#!/bin/sh

sudo apt-add-repository -y "ppa:ubuntu-toolchain-r/test"
sudo apt-get update
sudo apt-get install g++-6 unzip autoconf libtool libssl-dev python perl

git submodule update --init --recursive

mkdir cmake_app
wget --no-check-certificate --quiet -O - https://cmake.org/files/v3.7/cmake-3.7.2-Linux-x86_64.tar.gz | tar --strip-components=1 -xz -C cmake_app

export PROOT=$(pwd)
export PATH=`pwd`/cmake_app/bin:${PATH}
cmake --version

#install protobuf
cd 3rdparty/protobuf
./autogen.sh
./configure --prefix=$PROOT/3rdparty
make -j 4 && make install
cd ../..

#install mysql connector
./ci/install-mysql.sh

mkdir build
cd build
CC=gcc-6 CXX=g++-6 cmake -DENABLE_TESTING=ON -DENABLE_TESTING_COVERAGE=OFF ..

echo "Please add `pwd`/cmake_app/bin to your PATH env varaibles to ensure you will be able to compile in the future"
