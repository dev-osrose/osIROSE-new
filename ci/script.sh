#!/bin/sh

if [ "$SUITE" = "coverage" ]; then
	cmake -DENABLE_TESTING=ON -DENABLE_TESTING_COVERAGE=ON .. && make
	make coverage
	coveralls-lcov coverage.info.cleaned
else if [ "$SUITE" = "tests" ]; then
  cmake -DENABLE_TESTING=ON -DENABLE_TESTING_COVERAGE=OFF .. && make
  ctest --output-on-failure
else
	cmake .. && make
fi
