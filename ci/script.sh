#!/bin/sh

if [ "$SUITE" = "tests" ]; then
	cmake -DENABLE_TESTING=ON -DENABLE_TESTING_COVERAGE=ON .. && make
    make coverage
    coveralls-lcov coverage.info.cleaned
else
	cmake .. && make
fi
