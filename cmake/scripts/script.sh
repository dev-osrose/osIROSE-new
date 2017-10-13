#!/bin/sh

if [ "$SUITE" = "coverage" ]; then
#  cmake -GNinja -DBUILD_TESTS=ON -DWITH_COVERAGE_REPORTS=ON .. && cmake --build . && cmake --build . --target coverage && coveralls-lcov coverage.info.cleaned
  cmake -DBUILD_TESTS=ON -DWITH_COVERAGE_REPORTS=ON .. && cmake --build . -- -j 4 && cmake --build . --target coverage && coveralls-lcov coverage.info.cleaned
elif [ "$SUITE" = "tests" ]; then
  cmake -GNinja -DBUILD_TESTS=ON -DWITH_COVERAGE_REPORTS=OFF .. && cmake --build . -- -j 4 && ctest --output-on-failure
else
  cmake -GNinja -DBUILD_TESTS=OFF -DWITH_COVERAGE_REPORTS=OFF .. && cmake --build . -- -j 4
fi
