#!/bin/sh

sudo apt-add-repository -y "ppa:ubuntu-toolchain-r/test"
sudo apt-get update
sudo apt-get install g++-8 unzip autoconf libtool libssl-dev python perl libncurses5-dev libreadline-dev

git submodule update --init --recursive

if [ ! -d cmake_app ]; then
  mkdir cmake_app
  wget --no-check-certificate --quiet -O - https://github.com/Kitware/CMake/releases/download/v3.13.2/cmake-3.13.2-Linux-x86_64.tar.gz | tar --strip-components=1 -xz -C cmake_app
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
CC=gcc-8 CXX=g++-8 cmake -DENABLE_TESTING=ON -DENABLE_TESTING_COVERAGE=OFF ..

echo "Please add `pwd`/cmake_app/bin to your PATH env varaibles to ensure you will be able to compile in the future"
