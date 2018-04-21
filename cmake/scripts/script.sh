#!/bin/sh

if [ "$SUITE" = "coverage" ]; then
  cmake -GNinja -DWITH_COVERAGE_REPORTS=ON .. && cmake --build . --target all_unity -- -j 3 && mv bin/unity/* bin/ && cmake --build . --target coverage && coveralls-lcov coverage.info.cleaned
elif [ "$SUITE" = "tests" ]; then
  cmake -GNinja .. && cmake --build . --target all_unity -- -j 3 && mv bin/unity/* bin/ && ctest --output-on-failure
else
  cmake -GNinja -DOFFICIAL_BUILD=ON .. && cmake --build . -- -j 3
fi
