#!/bin/sh

if [ "$SUITE" = "coverage" ]; then
#  cmake -GNinja -DENABLE_TESTING=ON -DENABLE_TESTING_COVERAGE=ON -DBUILD_MYSQLPP=ON .. && ninja && ninja coverage && coveralls-lcov coverage.info.cleaned
  cmake -DENABLE_TESTING=ON -DENABLE_TESTING_COVERAGE=ON -DBUILD_MYSQLPP=ON .. && make && make coverage && coveralls-lcov coverage.info.cleaned
elif [ "$SUITE" = "tests" ]; then
  cmake -DENABLE_TESTING=ON -DENABLE_TESTING_COVERAGE=OFF -DBUILD_MYSQLPP=ON .. && make && ctest --output-on-failure
else
	cmake -GNinja .. && ninja
fi
