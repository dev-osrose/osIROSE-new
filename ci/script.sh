#!/bin/sh

if [ "$SUITE" = "coverage" ]; then
	cmake -GNinja -DENABLE_TESTING=ON -DENABLE_TESTING_COVERAGE=ON -DBUILD_MYSQLPP=ON .. && ninja
	ninja coverage
	coveralls-lcov coverage.info.cleaned
elif [ "$SUITE" = "tests" ]; then
  cmake -GNinja -DENABLE_TESTING=ON -DENABLE_TESTING_COVERAGE=OFF -DBUILD_MYSQLPP=ON .. && ninja
  ctest --output-on-failure
else
	cmake .. && make
fi
