#!/bin/sh

if [ "$SUITE" = "coverage" ]; then
  cmake -GNinja -DWITH_COVERAGE_REPORTS=ON .. && cmake --build . -- -j 4 && cmake --build . --target coverage && coveralls-lcov coverage.info.cleaned
elif [ "$SUITE" = "tests" ]; then
  cmake -GNinja .. && cmake --build . -- -j 4 && ctest --output-on-failure
else
  cmake -GNinja -DBUILD_TESTS=OFF .. && cmake --build . -- -j 4
fi
