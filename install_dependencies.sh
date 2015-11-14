#!/bin/bash

clear

echo "Downloading, building and installing dependencies."

mkdir 3rdParty
mkdir 3rdParty/include
mkdir 3rdParty/lib

echo "Installing cmake using apt-get"
sudo apt-get install -qq cmake # install the latest version of cmake
echo "Done installing cmake"

echo "Downloading boost 1.59, mysql 5.7.9 and mysql-connector 1.1.6"
mkdir dep
cd dep
wget http://iweb.dl.sourceforge.net/project/boost/boost/1.59.0/boost_1_59_0.tar.gz -O /tmp/boost-1.59.tar.gz
tar -xf /tmp/boost-1.59.tar.gz
wget http://cdn.mysql.com//Downloads/MySQL-5.7/mysql-5.7.9.tar.gz -O /tmp/mysql-5.7.9.tar.gz
tar -xf /tmp/mysql-5.7.9.tar.gz
wget http://dev.mysql.com/get/Downloads/Connector-C++/mysql-connector-c++-1.1.6.tar.gz -O /tmp/mysql-connector.tar.gz
tar -xf /tmp/mysql-connector.tar.gz
cp -r boost_1_59_0/boost ../3rdParty/include/

echo "Building mysql-5.7.9"
cd mysql-5.7.9
mkdir build
cd build
cmake -DWITH_BOOST=../../boost_1_59_0 ..
make > build.log
make install > install.log

echo "Building mysql-connector-c++-1.1.6"
cd ../../mysql-connector-c++-1.1.6
mkdir build
cd build
cmake ..
make > build.log
make install > install.log
cd ../..

echo "Finished building dependencies"