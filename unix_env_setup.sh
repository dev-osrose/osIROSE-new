#!/bin/sh

sudo apt-add-repository -y "ppa:ubuntu-toolchain-r/test"
sudo apt-get update
sudo apt-get install g++-7 unzip autoconf libtool libssl-dev python perl

git submodule update --init --recursive

if [ ! -d cmake_app ]; then
  mkdir cmake_app
  wget --no-check-certificate --quiet -O - https://cmake.org/files/v3.9/cmake-3.9.4-Linux-x86_64.tar.gz | tar --strip-components=1 -xz -C cmake_app
else
  echo "cmake installed already"
fi

export PROOT=$(pwd)
export PATH=`pwd`/cmake_app/bin:${PATH}
cmake --version

# sucks that we HAVE to do this to get a compile out
pip install --user pyparsing

mkdir build
cd build
CC=gcc-7 CXX=g++-7 cmake -DENABLE_TESTING=ON -DENABLE_TESTING_COVERAGE=OFF ..

echo "Please add `pwd`/cmake_app/bin to your PATH env varaibles to ensure you will be able to compile in the future"
