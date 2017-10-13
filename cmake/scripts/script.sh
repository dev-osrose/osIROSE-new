#!/bin/sh

if [ "$SUITE" = "coverage" ]; then
#  cmake -GNinja -DBUILD_TESTS=ON -DWITH_COVERAGE_REPORTS=ON .. && ninja && ninja coverage && coveralls-lcov coverage.info.cleaned
  cmake -DBUILD_TESTS=ON -DWITH_COVERAGE_REPORTS=ON .. && make && make coverage && coveralls-lcov coverage.info.cleaned
elif [ "$SUITE" = "tests" ]; then
  cmake -GNinja -DBUILD_TESTS=ON -DWITH_COVERAGE_REPORTS=OFF .. && ninja && ctest --output-on-failure
else
	cmake -GNinja .. && ninja
fi
