#!/bin/sh

if [ -z "$TRAVIS_OS_NAME" ]; then
    export TRAVIS_OS_NAME=linux
fi

if [ "$TRAVIS_OS_NAME" = "linux" ]; then
	export MAIN_PATH=$(pwd)
	sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
	sudo add-apt-repository -y "deb http://mirrors.kernel.org/ubuntu/ trusty main"
	sudo add-apt-repository -y "deb http://repo.mysql.com/apt/ubuntu/ precise mysql-5.7"
    sudo apt-get update -qq
    git submodule update --init --recursive
    
    export COVERALLS_SERVICE_NAME=travis-ci
    
    if [ "$CXX" = "g++" ]; then 
    	sudo apt-get install -qq "g++-${GCC_VERSION}"
    	export CXX="g++-${GCC_VERSION}" CC="gcc-${GCC_VERSION}" 
    fi
    
	if [ "$CXX" = "clang++" ]; then 
		sudo apt-get install -qq "clang-3.4"
		export CXX="clang++-3.4" CC="clang-3.4" 
	fi
	export BS_FLAGS="--cxx=$CXX"
	
# Install libc++
if [ -n "$LIBCXX" ]; then export BS_FLAGS="$BS_FLAGS --use-libcxx"; fi
if [ -n "$LIBCXX" -a "$CXX" == "clang++" ]; then svn co --quiet http://llvm.org/svn/llvm-project/libcxx/trunk libcxx; fi
if [ -n "$LIBCXX" -a "$CXX" == "clang++" ]; then cd libcxx/lib && bash buildit; fi
if [ -n "$LIBCXX" -a "$CXX" == "clang++" ]; then sudo cp ./libc++.so.1.0 /usr/lib/; fi
if [ -n "$LIBCXX" -a "$CXX" == "clang++" ]; then sudo mkdir /usr/include/c++/v1; fi
if [ -n "$LIBCXX" -a "$CXX" == "clang++" ]; then cd .. && sudo cp -r include/* /usr/include/c++/v1/; fi
if [ -n "$LIBCXX" -a "$CXX" == "clang++" ]; then cd /usr/lib && sudo ln -sf libc++.so.1.0 libc++.so; fi
if [ -n "$LIBCXX" -a "$CXX" == "clang++" ]; then sudo ln -sf libc++.so.1.0 libc++.so.1 && cd $cwd; fi

# Otherwise attempt to install g++ 4.8 libstdc++ instead for use in clang
if [ -z "$LIBCXX" -a "$CXX" == "clang++" ]; then sudo apt-get install -qq g++-4.9; fi

elif [ "$TRAVIS_OS_NAME" = "osx" ]; then
    brew update && brew --config
else
    echo "Unknown OS - '$TRAVIS_OS_NAME'. Stopping the build ..."
    exit 1
fi    
